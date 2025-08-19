#include "Characters/BaseCharacter.h"

// weapon
#include "Items/Weapons/Weapon.h"
#include "Components/BoxComponent.h"

ABaseCharacter::ABaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}

/*****************************************
 * INHERITED OVERRIDES OF BASIC GAMEPLAY *
 *****************************************/
void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();
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

/******************
 * COMBAT ATTACKS *
 ******************/

void ABaseCharacter::Attack(const FName& AttackType)
{
}
