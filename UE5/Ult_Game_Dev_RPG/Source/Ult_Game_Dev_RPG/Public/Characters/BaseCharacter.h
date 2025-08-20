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

  protected:
    virtual void BeginPlay() override;

    UAnimInstance* AnimInstance;

    /******************
     * WEAPON / ITEMS *
     ******************/
    UFUNCTION(BlueprintCallable)
    void SetWeaponCollisioneEnabled(ECollisionEnabled::Type CollisionEnabled);

    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    AWeapon* EquippedWeapon;

    /**************
     * COMPONENTS *
     **************/
    UPROPERTY(VisibleAnywhere)
    UAttributeComponent* Attributes;

    /***************************
     * COMBAT - DAMAGE / DEATH *
     ***************************/
    virtual bool CanAttack();
    virtual void Attack(const FName& AttackType);

    virtual void Die();
    virtual void HandleDamage(float DamageAmount);
    bool IsAlive();

    void DirectionalHitReact(const FVector& ImpactPoint);

    /**********************
     * ANIMATION MONTAGES *
     **********************/
    void PlayMontageSection(const FName& SectionName, UAnimMontage* AnimMontage);
    int32 PlayMontageSectionRandom(UAnimMontage* AnimMontage, const TArray<FName>& SectionNames);
    virtual int32 PlayMontageAttack();
    virtual int32 PlayMontageDeath();

    /*********************************
     * ANIMATION BLUEPRINT NOTIFIERS *
     *********************************/
    UFUNCTION(BlueprintCallable)
    virtual void AttackEnd();

    /************************************
     * ANIMATION BLUEPRINT NOTIFY NAMES *
     ************************************/
    const FName LightAttack = FName("LightAttack");
    const FName MediumAttack = FName("MediumAttack");
    const FName HeavyAttack = FName("HeavyAttack");

    const FName ReactFromBack = FName("FromBack");
    const FName ReactFromFront = FName("FromFront");
    const FName ReactFromLeft = FName("FromLeft");
    const FName ReactFromRight = FName("FromRight");

    /*************
     * SFX / VSF *
     *************/
    void PlayHitSound(const FVector& ImpactPoint);
    void SpawnHitParticles(const FVector& ImpactPoint);

    /*****************************
     * COMBAT HELPERS - INTERNAL *
     *****************************/
    void DisableCapsule();

  private:
    const FName EngageableTagName = FName("CombatAcceptable");

    /*************
     * SFX / VSF *
     *************/
    UPROPERTY(EditAnywhere, Category = "Sound Effects")
    USoundBase* HitSound;

    UPROPERTY(EditAnywhere, Category = "Visual Effects")
    UParticleSystem* HitParticles;

    /**********************
     * ANIMATION MONTAGES *
     **********************/
    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* AttackMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* HitReactMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* DeathMontage;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TArray<FName> AttackMontageSections;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TArray<FName> DeathMontageSections;

  public:
    FORCEINLINE FName GetEngageableTagName()
    {
        return EngageableTagName;
    }
};
