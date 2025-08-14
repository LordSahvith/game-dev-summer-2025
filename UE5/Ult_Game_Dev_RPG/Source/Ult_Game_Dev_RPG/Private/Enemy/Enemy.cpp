#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
    GetMesh()->SetGenerateOverlapEvents(true);

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    AnimInstance = GetMesh()->GetAnimInstance();
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
    DirectionalHitReact(ImpactPoint);

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
