#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"

AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
    GetMesh()->SetGenerateOverlapEvents(true);

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);

    Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

    HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar"));
    HealthBarWidget->SetupAttachment(GetRootComponent());
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    AnimInstance = GetMesh()->GetAnimInstance();

    if (HealthBarWidget)
    {
        HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
    }
}

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

/**
 * COMBAT MONTAGES
 */

void AEnemy::PlayMontageHitReact(const FName& AttackName)
{
    if (AnimInstance && HitReactMontage)
    {
        AnimInstance->Montage_Play(HitReactMontage);
        AnimInstance->Montage_JumpToSection(AttackName, HitReactMontage);
    }
}

/**
 * INTERFACES
 */

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
}

/**
 * HELPERS
 */

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
    const FVector Forward = GetActorForwardVector();
    const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
    const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

    // Forward * ToHit = cos(theta)
    const double CosTheta = FVector::DotProduct(Forward, ToHit);

    // Take the inverse cosine (arc-cosine) of cos(theta) to get theta
    // and convert from radians to degrees
    double Theta = FMath::RadiansToDegrees(FMath::Acos(CosTheta));

    // if cross product points down, Theta should be negative
    const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

    if (CrossProduct.Z < 0)
    {
        Theta *= -1.f;
    }

    if (Theta >= -45.f && Theta < 45.f)
    {
        PlayMontageHitReact(ReactFromFront);
    }
    else if (Theta >= -135.f && Theta < -45.f)
    {
        PlayMontageHitReact(ReactFromLeft);
    }
    else if (Theta >= 45.f && Theta < 135.f)
    {
        PlayMontageHitReact(ReactFromRight);
    }
    else
    {
        PlayMontageHitReact(ReactFromBack);
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

    return DamageAmount;
}

void AEnemy::Die()
{
    // TODO: play death montage
    if (AnimInstance && DeathMontage)
    {
        const int32 NumberOfAnimations = 6;
        const int32 Selection = FMath::RandRange(1, NumberOfAnimations);
        FString AttackName("Death");
        AttackName.AppendInt(Selection);

        AnimInstance->Montage_Play(DeathMontage);
        AnimInstance->Montage_JumpToSection(FName(AttackName), DeathMontage);
        DeathPose = GetDeathPose(Selection);
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
