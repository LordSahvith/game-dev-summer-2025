#include "Characters/BaseCharacter.h"
#include "Components/AttributeComponent.h"
#include "Animation/AnimMontage.h"

// weapon
#include "Items/Weapons/Weapon.h"
#include "Components/BoxComponent.h"

ABaseCharacter::ABaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

/*****************************************
 * INHERITED OVERRIDES OF BASIC GAMEPLAY *
 *****************************************/

void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    AnimInstance = GetMesh()->GetAnimInstance();
}

void ABaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABaseCharacter::SetWeaponCollisioneEnabled(ECollisionEnabled::Type CollisionEnabled)
{
    if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
    {
        EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
        EquippedWeapon->IgnoreActors.Empty();
    }
}

/***************************
 * COMBAT - DAMAGE / DEATH *
 ***************************/

void ABaseCharacter::Attack(const FName& AttackType)
{
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
}

void ABaseCharacter::Die()
{
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
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

/*********************
 * COMBAT - MONTAGES *
 *********************/

void ABaseCharacter::PlayMontageAttack(const FName& AttackName, UAnimMontage* AnimMontage)
{
}

void ABaseCharacter::PlayMontageHitReact(const FName& AttackName)
{
    if (AnimInstance && HitReactMontage)
    {
        AnimInstance->Montage_Play(HitReactMontage);
        AnimInstance->Montage_JumpToSection(AttackName, HitReactMontage);
    }
}

/**************************************************
 * COMBAT HELPERS - ANIMATION BLUEPRINT NOTIFIERS *
 **************************************************/

void ABaseCharacter::AttackEnd()
{
}

/*****************************
 * COMBAT HELPERS - INTERNAL *
 *****************************/

bool ABaseCharacter::CanAttack()
{
    return false;
}
