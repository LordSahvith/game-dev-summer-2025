// Copyright Lord Savith
#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
    GENERATED_BODY()

  public:
    AAuraEnemy();

    /**
     * @interface: IActorHoverInterface
     */
    virtual void HighlightActor() override;
    virtual void UnHighlightActor() override;

    /**
     * @interface: ICombatInterface
     */
    virtual int32 GetPlayerLevel() override;

    UPROPERTY(BlueprintAssignable)
    FOnAttributeChangedSignature OnHealthChanged;

    UPROPERTY(BlueprintAssignable)
    FOnAttributeChangedSignature OnMaxHealthChanged;

  protected:
    virtual void BeginPlay() override;
    virtual void InitAbilityActorInfo() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
    int32 Level = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UWidgetComponent> HealthBar;
};
