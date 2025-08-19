#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;

UCLASS()
class ULT_GAME_DEV_RPG_API ABaseCharacter : public ACharacter, public IHitInterface
{
    GENERATED_BODY()

  public:
    ABaseCharacter();

    /*****************************************
     * INHERITED OVERRIDES OF BASIC GAMEPLAY *
     *****************************************/
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    void SetWeaponCollisioneEnabled(ECollisionEnabled::Type CollisionEnabled);

  protected:
    virtual void BeginPlay() override;

    /******************
     * WEAPON / ITEMS *
     ******************/
    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    AWeapon* EquippedWeapon;

    /**********
     * COMBAT *
     **********/
    virtual void Attack(const FName& AttackType);

    /**********************
     * ANIMATION MONTAGES *
     **********************/
    // UPROPERTY(EditDefaultsOnly, Category = "Montages")
    // UAnimMontage* OneHandedAttackMontage;

    /**************************
     * PLAY MONTAGE FUNCTIONS *
     **************************/
    // void PlayMontageOneHandedAttack(const FName& AttackName);

  private:
};
