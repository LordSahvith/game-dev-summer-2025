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

UCLASS()
class ULT_GAME_DEV_RPG_API AMainCharacter : public ABaseCharacter
{
    GENERATED_BODY()

  public:
    AMainCharacter();
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    /**
     * @interface: IHitInterface
     */
    virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

  protected:
    virtual void BeginPlay() override;

    /*********************************
     * ANIMATION BLUEPRINT NOTIFIERS *
     *********************************/
    virtual void AttackEnd() override;

    /*****************
     * INPUT ACTIONS *
     *****************/
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* CharacterMappingContext;

    UFUNCTION(BlueprintCallable)
    void SheathEnd();

    UFUNCTION(BlueprintCallable)
    void AttachWeaponToBack();

    UFUNCTION(BlueprintCallable)
    void AttachWeaponToHand();

  private:
    /**********
     * STATES *
     **********/
    ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

    UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    EActionState ActionState = EActionState::EAS_Unoccupied;

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

    bool CanSheathWeapon();
    bool CanDrawWeapon();

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
    virtual bool CanAttack() override;
    void EquipWeapon(AWeapon* Weapon);
    void SheathWeapon();
    void DrawWeapon();

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

    /**********************
     * ANIMATION MONTAGES *
     **********************/
    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* EquipMontage;

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
