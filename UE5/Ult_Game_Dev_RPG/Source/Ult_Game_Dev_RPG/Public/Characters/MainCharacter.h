#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
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
class ULT_GAME_DEV_RPG_API AMainCharacter : public ABaseCharacter
{
    GENERATED_BODY()

  public:
    AMainCharacter();

    /*****************************************
     * INHERITED OVERRIDES OF BASIC GAMEPLAY *
     *****************************************/
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

  protected:
    virtual void BeginPlay() override;

    /*****************
     * INPUT ACTIONS *
     *****************/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* CharacterMappingContext;

    /*********************************
     * ANIMATION BLUEPRINT NOTIFIERS *
     *********************************/
    UFUNCTION(BlueprintCallable)
    void AttackEnd();

    UFUNCTION(BlueprintCallable)
    void SheathEnd();

    UFUNCTION(BlueprintCallable)
    void SheathWeapon();

    UFUNCTION(BlueprintCallable)
    void DrawWeapon();

    /***********
     * HELPERS *
     **********/
    UFUNCTION(BlueprintCallable)
    bool CanAttack();

    bool CanSheathWeapon();
    bool CanDrawWeapon();

  private:
    /**********
     * STATES *
     **********/
    ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

    UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    EActionState ActionState = EActionState::EAS_Unoccupied;

    UAnimInstance* AnimInstance;

    /**********
     * SCENES *
     **********/
    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* ViewCamera;

    UPROPERTY(VisibleAnywhere, Category = "Hair")
    UGroomComponent* Hair;

    UPROPERTY(VisibleAnywhere, Category = "Hair")
    UGroomComponent* Eyebrows;

    /*********
     * INPUT *
     *********/
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* EquipAction;

    /***********************
     * CALLBACKS FOR INPUT *
     ***********************/
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    virtual void Jump() override;
    void Equip(const FInputActionValue& Value);
    void AttackLight(const FInputActionValue& Value);
    void AttackMedium(const FInputActionValue& Value);
    void AttackHeavy(const FInputActionValue& Value);

    /**********
     * COMBAT *
     **********/
    virtual void Attack(const FName& AttackType) override;

    /***********
     * ATTACKS *
     ***********/
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* AttackLightAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* AttackMediumAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* AttackHeavyAction;

    /******************
     * WEAPON / ITEMS *
     ******************/
    UPROPERTY(VisibleInstanceOnly)
    AItem* OverlappingItem;

    /***********************
     * WEAPON SOCKET NAMES *
     ***********************/
    UPROPERTY(EditAnywhere, Category = "Weapon")
    FName EquippedSocket = FName("RightHandSocket");

    UPROPERTY(EditAnywhere, Category = "Weapon")
    FName SheathedSocket = FName("SpineSocket");

    UPROPERTY(EditAnywhere, Category = "Weapon")
    FName SheathWeaponName = FName("SheathWeapon");

    UPROPERTY(EditAnywhere, Category = "Weapon")
    FName DrawWeaponName = FName("DrawWeapon");

    /************************************
     * ANIMATION BLUEPRINT NOTIFY NAMES *
     ************************************/
    const FName LightAttack = FName("LightAttack");
    const FName MediumAttack = FName("MediumAttack");
    const FName HeavyAttack = FName("HeavyAttack");

    /**********************
     * ANIMATION MONTAGES *
     **********************/
    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* JumpMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* OneHandedAttackMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* EquipMontage;

    /**************************
     * PLAY MONTAGE FUNCTIONS *
     **************************/
    void PlayMontageOneHandedAttack(const FName& AttackName);
    void PlayMontageEquip(const FName& SectionName);

  public:
    /***********
     * GETTERS *
     ***********/
    FORCEINLINE void SetOverlappingItem(AItem* Item)
    {
        OverlappingItem = Item;
    }

    FORCEINLINE ECharacterState GetCharacterState() const
    {
        return CharacterState;
    }
};
