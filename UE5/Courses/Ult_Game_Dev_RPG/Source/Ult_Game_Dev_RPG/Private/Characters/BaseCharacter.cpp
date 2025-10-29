#include "Characters/BaseCharacter.h"
#include "Components/AttributeComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// weapon
#include "Items/Weapons/Weapon.h"
#include "Components/BoxComponent.h"

ABaseCharacter::ABaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
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

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
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
bool ABaseCharacter::CanAttack()
{
    return false;
}

void ABaseCharacter::Attack(const FName& AttackType)
{
    if (AttackMontage)
    {
        PlayMontageSection(AttackType, AttackMontage);
    }
}

void ABaseCharacter::Die()
{
}

// interface
void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
    if (Attributes)
    {
        Attributes->RecieveDamage(DamageAmount);
    }
}

bool ABaseCharacter::IsAlive()
{
    return Attributes && Attributes->IsAlive();
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
        PlayMontageSection(ReactFromFront, HitReactMontage);
    }
    else if (Theta >= -135.f && Theta < -45.f)
    {
        PlayMontageSection(ReactFromLeft, HitReactMontage);
    }
    else if (Theta >= 45.f && Theta < 135.f)
    {
        PlayMontageSection(ReactFromRight, HitReactMontage);
    }
    else
    {
        PlayMontageSection(ReactFromBack, HitReactMontage);
    }
}

/*********************
 * COMBAT - MONTAGES *
 *********************/
void ABaseCharacter::PlayMontageSection(const FName& SectionName, UAnimMontage* AnimMontage)
{
    if (AnimInstance && AnimMontage)
    {
        AnimInstance->Montage_Play(AnimMontage);
        AnimInstance->Montage_JumpToSection(SectionName, AnimMontage);
    }
}

int32 ABaseCharacter::PlayMontageSectionRandom(UAnimMontage* AnimMontage, const TArray<FName>& SectionNames)
{
    if (SectionNames.Num() <= 0) return -1;

    int32 MaxSectionIndex = SectionNames.Num() - 1;
    int32 Selection = FMath::RandRange(0, MaxSectionIndex);

    PlayMontageSection(SectionNames[Selection], AnimMontage);
    return Selection;
}

int32 ABaseCharacter::PlayMontageAttack()
{
    return PlayMontageSectionRandom(AttackMontage, AttackMontageSections);
}

int32 ABaseCharacter::PlayMontageDeath()
{
    return PlayMontageSectionRandom(DeathMontage, DeathMontageSections);
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
void ABaseCharacter::DisableCapsule()
{
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

/*************
 * SFX / VSF *
 *************/
void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
    if (HitSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
    }
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
    if (GetWorld() && HitParticles)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
    }
}
