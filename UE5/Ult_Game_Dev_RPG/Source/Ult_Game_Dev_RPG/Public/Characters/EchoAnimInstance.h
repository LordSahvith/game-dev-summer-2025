#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "EchoAnimInstance.generated.h"

class AMainCharacter;
class UCharacterMovementComponent;

UCLASS()
class ULT_GAME_DEV_RPG_API UEchoAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

  public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaTime) override;

    UPROPERTY(BlueprintReadOnly)
    AMainCharacter* MainCharacter;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    UCharacterMovementComponent* CharacterMovement;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float GroundSpeed;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool IsFalling;

    UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State")
    ECharacterState CharacterState;
};
