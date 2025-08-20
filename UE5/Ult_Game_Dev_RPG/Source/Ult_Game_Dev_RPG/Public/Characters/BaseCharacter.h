#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;

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

    // Interface
    virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

    UFUNCTION(BlueprintCallable)
    void SetWeaponCollisioneEnabled(ECollisionEnabled::Type CollisionEnabled);

  protected:
    virtual void BeginPlay() override;
    virtual void Die();

    UAnimInstance* AnimInstance;

    /******************
     * WEAPON / ITEMS *
     ******************/
    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    AWeapon* EquippedWeapon;

    /**************
     * COMPONENTS *
     **************/
    UPROPERTY(VisibleAnywhere)
    UAttributeComponent* Attributes;

    /**********
     * COMBAT *
     **********/
    virtual void Attack(const FName& AttackType);
    virtual bool CanAttack();

    /************************************
     * ANIMATION BLUEPRINT NOTIFY NAMES *
     ************************************/
    const FName LightAttack = FName("LightAttack");
    const FName MediumAttack = FName("MediumAttack");
    const FName HeavyAttack = FName("HeavyAttack");

    /******************
     * DAMAGE / DEATH *
     ******************/
    void DirectionalHitReact(const FVector& ImpactPoint);

    /**********************
     * ANIMATION MONTAGES *
     **********************/
    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* AttackMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* HitReactMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* DeathMontage;

    /**************************
     * PLAY MONTAGE FUNCTIONS *
     **************************/
    virtual void PlayMontageAttack(const FName& AttackName, UAnimMontage* AnimMontage);
    void PlayMontageHitReact(const FName& AttackName);

    /*********************************
     * ANIMATION BLUEPRINT NOTIFIERS *
     *********************************/
    UFUNCTION(BlueprintCallable)
    virtual void AttackEnd();

    /************************************
     * ANIMATION BLUEPRINT NOTIFY NAMES *
     ************************************/
    const FName ReactFromBack = FName("FromBack");
    const FName ReactFromFront = FName("FromFront");
    const FName ReactFromLeft = FName("FromLeft");
    const FName ReactFromRight = FName("FromRight");

    /*************
     * SFX / VSF *
     *************/
    UPROPERTY(EditAnywhere, Category = "Sound Effects")
    USoundBase* HitSound;

    UPROPERTY(EditAnywhere, Category = "Visual Effects")
    UParticleSystem* HitParticles;

  private:
};
