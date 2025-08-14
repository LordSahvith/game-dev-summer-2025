#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "MainCharacter.generated.h"

class UInputComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class UAnimMontage;
class AWeapon;

UCLASS()
class ULT_GAME_DEV_RPG_API AMainCharacter : public ACharacter
{
    GENERATED_BODY()

  public:
    AMainCharacter();
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable)
    void SetWeaponCollisioneEnabled(ECollisionEnabled::Type CollisionEnabled);

  protected:
    virtual void BeginPlay() override;

    /**
     * Input Actions
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* CharacterMappingContext;

    /**
     * Helpers for Animation Blueprint Notifiers
     */
    UFUNCTION(BlueprintCallable)
    void AttackEnd();

    UFUNCTION(BlueprintCallable)
    void SheathEnd();

    UFUNCTION(BlueprintCallable)
    void SheathWeapon();

    UFUNCTION(BlueprintCallable)
    void DrawWeapon();

    /**
     * Misc.
     */
    UFUNCTION(BlueprintCallable)
    bool CanAttack();

    bool CanSheathWeapon();
    bool CanDrawWeapon();

  private:
    /**
     * States
     */
    ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

    UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    EActionState ActionState = EActionState::EAS_Unoccupied;

    UAnimInstance* AnimInstance;

    /**
     * Scenes
     */
    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* ViewCamera;

    UPROPERTY(VisibleAnywhere, Category = "Hair")
    UGroomComponent* Hair;

    UPROPERTY(VisibleAnywhere, Category = "Hair")
    UGroomComponent* Eyebrows;

    /**
     * INPUT
     */

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* EquipAction;

    /**
     * Callbacks for Input
     */
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    virtual void Jump() override;
    void Equip(const FInputActionValue& Value);
    void AttackBasic(const FInputActionValue& Value);
    void AttackThreePartCombo(const FInputActionValue& Value);
    void AttackCircle(const FInputActionValue& Value);
    void AttackHeavy(const FInputActionValue& Value);

    /**
     * Attacks
     */
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* AttackBasicAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* AttackThreePartComboAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* AttackCircleAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* AttackHeavyAction;

    /**
     * Weapon / Items
     */
    UPROPERTY(VisibleInstanceOnly)
    AItem* OverlappingItem;

    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    AWeapon* EquippedWeapon;

    /**
     * Weapon Sockets Names
     */
    UPROPERTY(EditAnywhere, Category = "Weapon")
    FName EquippedSocket = FName("RightHandSocket");

    UPROPERTY(EditAnywhere, Category = "Weapon")
    FName SheathedSocket = FName("SpineSocket");

    UPROPERTY(EditAnywhere, Category = "Weapon")
    FName SheathWeaponName = FName("SheathWeapon");

    UPROPERTY(EditAnywhere, Category = "Weapon")
    FName DrawWeaponName = FName("DrawWeapon");

    /**
     * Combat
     */
    void Attack(const FName& AttackType);

    /**
     * Animation Blueprint Notify Names
     */
    const FName BasicAttack = FName("BasicAttack");
    const FName CircleAttack = FName("CircleAttack");
    const FName HeavyAttack = FName("HeavyAttack");

    /**
     * Animation Montages
     */
    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* JumpMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* OneHandedAttackMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* EquipMontage;

    /**
     * Play Montage Functions
     */
    void PlayMontageOneHandedAttack(const FName& AttackName);
    void PlayMontageEquip(const FName& SectionName);

  public:
    FORCEINLINE void SetOverlappingItem(AItem* Item)
    {
        OverlappingItem = Item;
    }

    FORCEINLINE ECharacterState GetCharacterState() const
    {
        return CharacterState;
    }
};
