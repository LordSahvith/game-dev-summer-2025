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

  protected:
    virtual void BeginPlay() override;

    /**
     * Input Actions
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* CharacterMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* EquipAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* AttackAction;

    /**
     * Callbacks for Input
     */
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    virtual void Jump() override;
    void Equip(const FInputActionValue& Value);
    void Attack(const FInputActionValue& Value);

    /**
     * Animation Montages
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montages")
    UAnimMontage* JumpMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montages")
    UAnimMontage* OneHandedAttackMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montages")
    UAnimMontage* EquipMontage;

    /**
     * Play Montage Functions
     */
    void PlayMontageOneHandedAttack();
    void PlayMontageEquip(FName SectionName);

    UFUNCTION(BlueprintCallable)
    void AttackEnd();

    UFUNCTION(BlueprintCallable)
    void EquipEnd();

    UFUNCTION(BlueprintCallable)
    bool CanAttack();

    bool CanDisarm();
    bool CanArm();

  private:
    ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

    UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    EActionState ActionState = EActionState::EAS_Unoccupied;

    UAnimInstance* AnimInstance;

    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* ViewCamera;

    UPROPERTY(VisibleAnywhere, Category = "Hair")
    UGroomComponent* Hair;

    UPROPERTY(VisibleAnywhere, Category = "Hair")
    UGroomComponent* Eyebrows;

    UPROPERTY(VisibleInstanceOnly)
    AItem* OverlappingItem;

    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    AWeapon* EquippedWeapon;

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
