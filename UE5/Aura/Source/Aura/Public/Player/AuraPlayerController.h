// Copyright Lord Savith
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class IActorHoverInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;

struct FInputActionValue;

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
    GENERATED_BODY()

  public:
    AAuraPlayerController();
    virtual void PlayerTick(float DeltaTime) override;

  protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

  private:
    UPROPERTY(Editanywhere, Category = "Input")
    TObjectPtr<UInputMappingContext> AuraContext;

    UPROPERTY(Editanywhere, Category = "Input")
    TObjectPtr<UInputAction> MoveAction;

    void Move(const FInputActionValue& InputActionValue);

    void CursorTrace();

    TScriptInterface<IActorHoverInterface> LastActor;
    TScriptInterface<IActorHoverInterface> CurrentActor;
    FHitResult CursorHit;

    void AbilityInputTagPressed(FGameplayTag InputTag);
    void AbilityInputTagReleased(FGameplayTag InputTag);
    void AbilityInputTagHeld(FGameplayTag InputTag);

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UAuraInputConfig> InputConfig;

    UPROPERTY()
    TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

    UAuraAbilitySystemComponent* GetASC();

    FVector CachedDestination{FVector::ZeroVector};
    float FollowTime{0.f};
    float ShortPressThreshold{0.5f};
    bool bAutoRunning{false};
    bool bTargeting{false};

    UPROPERTY(EditDefaultsOnly)
    float AutoRunAcceptanceRadius{50.f};

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USplineComponent> Spline;

    void AutoRun();
};
