// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MainCharacter.generated.h"

class UInputComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;

UCLASS()
class ULT_GAME_DEV_RPG_API AMainCharacter : public ACharacter
{
    GENERATED_BODY()

  public:
    AMainCharacter();
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    // virtual void Jump() override;

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputMappingContext* CharacterMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* EquipAction;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Equip(const FInputActionValue& Value);

  private:
    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* ViewCamera;

    UPROPERTY(VisibleAnywhere, Category = Hair)
    UGroomComponent* Hair;

    UPROPERTY(VisibleAnywhere, Category = Hair)
    UGroomComponent* Eyebrows;

    UPROPERTY(VisibleInstanceOnly)
    AItem* OverlappingItem;

  public:
    FORCEINLINE void SetOverlappingItem(AItem* Item)
    {
        OverlappingItem = Item;
    }
};
