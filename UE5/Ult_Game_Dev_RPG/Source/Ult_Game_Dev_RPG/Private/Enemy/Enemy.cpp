#include "Enemy/Enemy.h"
#include "AIController.h"
#include "HUD/HealthBarComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Components/BoxComponent.h"

AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
    GetMesh()->SetGenerateOverlapEvents(true);

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);

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

    if (HealthBarWidget)
    {
        HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
        HealthBarWidget->SetVisibility(false);
    }

    EnemyController = Cast<AAIController>(GetController());

    MoveToTarget(PatrolTarget);

    if (PawnSensing)
    {
        PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
    }

    UWorld* World = GetWorld();

    if (World && WeaponClass)
    {
        AWeapon* DefaultWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
        DefaultWeapon->Equip(GetMesh(), EquippedSocket, this, this);
        EquippedWeapon = DefaultWeapon;
    }
}

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (EnemyState > EEnemyState::EES_Patrolling)
    {
        CheckCombatTarget();
    }
    else
    {
        CheckPatrolTarget();
    }
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

/******************
 * DAMAGE / DEATH *
 ******************/

// Interface
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
    if (Attributes && Attributes->IsAlive())
    {
        DirectionalHitReact(ImpactPoint);
    }
    else if (Attributes && !Attributes->IsAlive())
    {
        Die();
    }

    if (HitSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
    }

    if (GetWorld() && HitParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
    }

    if (HealthBarWidget)
    {
        HealthBarWidget->SetVisibility(true);
    }
}

float AEnemy::TakeDamage(float DamageAmount,
                         struct FDamageEvent const& DamageEvent,
                         class AController* EventInstigator,
                         AActor* DamageCauser)
{
    if (Attributes && HealthBarWidget)
    {
        Attributes->RecieveDamage(DamageAmount);
        HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
    }

    CombatTarget = EventInstigator->GetPawn();
    EnemyState = EEnemyState::EES_Chasing;
    GetCharacterMovement()->MaxWalkSpeed = Attributes->GetMaxRunSpeed();
    MoveToTarget(CombatTarget);

    return DamageAmount;
}

void AEnemy::Die()
{
    if (AnimInstance && DeathMontage)
    {
        const int32 NumberOfAnimations = 6;
        const int32 Selection = FMath::RandRange(1, NumberOfAnimations);
        FString AttackName("Death");
        AttackName.AppendInt(Selection);

        AnimInstance->Montage_Play(DeathMontage);
        AnimInstance->Montage_JumpToSection(FName(AttackName), DeathMontage);
        DeathPose = GetDeathPose(Selection);

        if (HealthBarWidget)
        {
            HealthBarWidget->SetVisibility(false);
        }

        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        SetLifeSpan(5.f);
    }
}

EDeathPose AEnemy::GetDeathPose(int32 PoseType)
{
    EDeathPose Pose;

    switch (PoseType)
    {
        case 1:
            Pose = EDeathPose::EDP_Death1;
            break;
        case 2:
            Pose = EDeathPose::EDP_Death2;
            break;
        case 3:
            Pose = EDeathPose::EDP_Death3;
            break;
        case 4:
            Pose = EDeathPose::EDP_Death4;
            break;
        case 5:
            Pose = EDeathPose::EDP_Death5;
            break;
        case 6:
            Pose = EDeathPose::EDP_Death6;
            break;
        default:
            Pose = EDeathPose::EDP_Alive;
            break;
    }

    return Pose;
}

void AEnemy::Destroyed()
{
    if (EquippedWeapon)
    {
        EquippedWeapon->Destroy();
    }
}

/***************************
 * PATROLLING / NAVIGATION *
 ***************************/

void AEnemy::PatrolTimerFinished()
{
    MoveToTarget(PatrolTarget);
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
    if (EnemyState == EEnemyState::EES_Chasing) return;

    if (SeenPawn->ActorHasTag(FName("MainCharacter")))
    {
        GetWorldTimerManager().ClearTimer(PatrolTimer);
        GetCharacterMovement()->MaxWalkSpeed = Attributes->GetMaxRunSpeed();
        CombatTarget = SeenPawn;

        if (EnemyState != EEnemyState::EES_Attacking)
        {
            EnemyState = EEnemyState::EES_Chasing;
            MoveToTarget(CombatTarget);
        }
    }
}

/**********
 * COMBAT *
 **********/

void AEnemy::Attack(const FName& AttackType)
{
    Super::Attack(AttackType);

    PlayMontageAttack(AttackType, AttackMontage);
}

/*******************
 * COMBAT MONTAGES *
 *******************/

void AEnemy::PlayMontageAttack(const FName& AttackName, UAnimMontage* AnimMontage)
{
    Super::PlayMontageAttack(AttackName, AnimMontage);

    if (AnimInstance && AnimMontage)
    {
        AnimInstance->Montage_Play(AnimMontage);
        AnimInstance->Montage_JumpToSection(AttackName, AnimMontage);
    }
}

/***********
 * HELPERS *
 ***********/

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
    if (Target == nullptr) return false;

    const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
    return DistanceToTarget <= Radius;
}

void AEnemy::CheckCombatTarget()
{
    if (!InTargetRange(CombatTarget, CombatRadius) && EnemyState != EEnemyState::EES_Patrolling)
    {
        // outside combat radius, lose interest
        CombatTarget = nullptr;

        if (HealthBarWidget)
        {
            HealthBarWidget->SetVisibility(false);
        }

        EnemyState = EEnemyState::EES_Patrolling;
        GetCharacterMovement()->MaxWalkSpeed = Attributes->GetMaxWalkSpeed();
        MoveToTarget(PatrolTarget);
    }
    else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
    {
        // outside attack range, chase character
        EnemyState = EEnemyState::EES_Chasing;
        GetCharacterMovement()->MaxWalkSpeed = Attributes->GetMaxRunSpeed();
        MoveToTarget(CombatTarget);
    }
    else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
    {
        // inside attack range, attack player
        EnemyState = EEnemyState::EES_Attacking;

        int32 Selection = FMath::RandRange(0, 2);
        switch (Selection)
        {
            case 0:
                Attack(LightAttack);
                break;
            case 1:
                Attack(MediumAttack);
                break;
            case 2:
                Attack(HeavyAttack);
                break;
        }
    }
}

void AEnemy::CheckPatrolTarget()
{
    if (InTargetRange(PatrolTarget, PatrolRadius))
    {
        PatrolTarget = ChoosePatrolTarget();
        const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
        GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
    }
}
