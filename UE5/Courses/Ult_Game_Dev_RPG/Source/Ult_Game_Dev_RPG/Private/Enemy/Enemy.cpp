#include "Enemy/Enemy.h"

// custom components
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"

// engine components
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// AI
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"

// weapon
#include "Items/Weapons/Weapon.h"
#include "Components/BoxComponent.h"

AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
    GetMesh()->SetGenerateOverlapEvents(true);

    HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar"));
    HealthBarWidget->SetupAttachment(GetRootComponent());

    GetCharacterMovement()->bOrientRotationToMovement = true;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

    PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing"));
    PawnSensing->SightRadius = 4000.f;
    PawnSensing->SetPeripheralVisionAngle(45.f);
}

/*****************************************
 * INHERITED OVERRIDES OF BASIC GAMEPLAY *
 *****************************************/
void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    if (PawnSensing)
    {
        PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
    }

    InitializeEnemy();

    Tags.Add(GetEnemyTagName());
}

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsDead()) return;

    if (EnemyState > EEnemyState::EES_Patrolling)
    {
        CheckCombatTarget();
    }
    else
    {
        CheckPatrolTarget();
    }
}

/***************************
 * COMBAT - DAMAGE / DEATH *
 ***************************/
// Interface
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
    ShowHealthBar();
    IsAlive() ? DirectionalHitReact(ImpactPoint) : Die();
    PlayHitSound(ImpactPoint);
    SpawnHitParticles(ImpactPoint);
}

float AEnemy::TakeDamage(float DamageAmount,
                         struct FDamageEvent const& DamageEvent,
                         class AController* EventInstigator,
                         AActor* DamageCauser)
{
    HandleDamage(DamageAmount);
    CombatTarget = EventInstigator->GetPawn();
    ChaseTarget();

    return DamageAmount;
}

void AEnemy::Die()
{
    EnemyState = EEnemyState::EES_Dead;
    ClearAttackTimer();
    HideHealthBar();
    DisableCapsule();
    SetLifeSpan(DeathLifeSpan);
    PlayMontageDeath();
}

void AEnemy::Destroyed()
{
    if (EquippedWeapon)
    {
        EquippedWeapon->Destroy();
    }
}

bool AEnemy::CanAttack()
{
    return IsInsideAttackRadius() && !IsAttacking() && !IsEngaged() && !IsDead();
}

void AEnemy::AttackTarget()
{
    EnemyState = EEnemyState::EES_Engaged;
    PlayMontageAttack();
}

void AEnemy::StartAttackTimer()
{
    EnemyState = EEnemyState::EES_Attacking;
    const float AttackTime = FMath::RandRange(AttackWaitMin, AttackWaitMax);
    GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::AttackTarget, AttackTime);
}

void AEnemy::ClearAttackTimer()
{
    GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemy::HandleDamage(float DamageAmount)
{
    Super::HandleDamage(DamageAmount);

    if (HealthBarWidget)
    {
        HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
    }
}

void AEnemy::SpawnDefaultWeapon()
{
    if (GetWorld() && WeaponClass)
    {
        AWeapon* DefaultWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
        DefaultWeapon->Equip(GetMesh(), EquippedSocket, this, this);
        EquippedWeapon = DefaultWeapon;
    }
}

/***************************
 * PATROLLING / NAVIGATION *
 ***************************/
void AEnemy::PatrolTimerFinished()
{
    MoveToTarget(PatrolTarget);
}

void AEnemy::StartPatrolTimer()
{
    const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
    GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
}

void AEnemy::ClearPatrolTimer()
{
    GetWorldTimerManager().ClearTimer(PatrolTimer);
}

AActor* AEnemy::ChoosePatrolTarget()
{
    TArray<AActor*> ValidTargets;
    for (AActor* Target : PatrolTargets)
    {
        if (Target != PatrolTarget)
        {
            ValidTargets.AddUnique(Target);
        }
    }

    const int32 PatrolTargetCount = ValidTargets.Num();
    if (PatrolTargetCount > 0)
    {
        const int32 TargetSelection = FMath::RandRange(0, PatrolTargetCount - 1);
        return ValidTargets[TargetSelection];
    }

    return nullptr;
}

void AEnemy::MoveToTarget(AActor* Target)
{
    if (EnemyController == nullptr || Target == nullptr) return;

    FAIMoveRequest MoveRequest;
    MoveRequest.SetGoalActor(Target);
    MoveRequest.SetAcceptanceRadius(50.f);

    EnemyController->MoveTo(MoveRequest);
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
    const bool bShouldChaseTarget = !IsDead() && !IsChasing() && EnemyState < EEnemyState::EES_Attacking &&
                                    SeenPawn->ActorHasTag(GetEngageableTagName());

    if (bShouldChaseTarget)
    {
        CombatTarget = SeenPawn;
        ClearPatrolTimer();
        ChaseTarget();
    }
}

/**********************
 * ANIMATION MONTAGES *
 **********************/
int32 AEnemy::PlayMontageDeath()
{
    const int32 Selection = Super::PlayMontageDeath();
    EDeathPose Pose = static_cast<EDeathPose>(Selection);

    if (Pose < EDeathPose::EDP_MAX)
    {
        DeathPose = Pose;
    }

    return Selection;
}

/*********************************
 * ANIMATION BLUEPRINT NOTIFIERS *
 *********************************/
void AEnemy::AttackEnd()
{
    EnemyState = EEnemyState::EES_NoState;
    CheckCombatTarget();
}

/***************
 * AI BEHAVIOR *
 ***************/
void AEnemy::InitializeEnemy()
{
    EnemyController = Cast<AAIController>(GetController());
    MoveToTarget(PatrolTarget);
    HideHealthBar();
    SpawnDefaultWeapon();
}

void AEnemy::ToggleHealthBar(bool bShouldShow)
{
    if (HealthBarWidget)
    {
        HealthBarWidget->SetVisibility(bShouldShow);
    }
}

void AEnemy::HideHealthBar()
{
    ToggleHealthBar(false);
}

void AEnemy::ShowHealthBar()
{
    ToggleHealthBar(true);
}

void AEnemy::LoseInterest()
{
    CombatTarget = nullptr;
    HideHealthBar();
}

void AEnemy::StartPatrolling()
{
    EnemyState = EEnemyState::EES_Patrolling;
    GetCharacterMovement()->MaxWalkSpeed = Attributes->GetMaxWalkSpeed();
    MoveToTarget(PatrolTarget);
}

void AEnemy::ChaseTarget()
{
    EnemyState = EEnemyState::EES_Chasing;
    GetCharacterMovement()->MaxWalkSpeed = Attributes->GetMaxRunSpeed();
    MoveToTarget(CombatTarget);
}

bool AEnemy::IsOutsideCombatRadius()
{
    return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
    return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
    return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsDead()
{
    return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsPatrolling()
{
    return EnemyState == EEnemyState::EES_Patrolling;
}

bool AEnemy::IsChasing()
{
    return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsAttacking()
{
    return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsEngaged()
{
    return EnemyState == EEnemyState::EES_Engaged;
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
    if (Target == nullptr) return false;

    const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
    return DistanceToTarget <= Radius;
}

void AEnemy::CheckCombatTarget()
{
    if (IsOutsideCombatRadius() && !IsPatrolling())
    {
        ClearAttackTimer();
        LoseInterest();

        if (!IsEngaged())
        {
            StartPatrolling();
        }
    }
    else if (IsOutsideAttackRadius() && !IsChasing())
    {
        ClearAttackTimer();

        if (!IsEngaged())
        {
            ChaseTarget();
        }
    }
    else if (CanAttack())
    {
        StartAttackTimer();
    }
}

void AEnemy::CheckPatrolTarget()
{
    if (InTargetRange(PatrolTarget, PatrolRadius))
    {
        PatrolTarget = ChoosePatrolTarget();
        StartPatrolTimer();
    }
}
