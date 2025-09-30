// Copyright Lord Savith
#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
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
    virtual int32 GetPlayerLevel() const override;
    virtual void Die() override;

    UPROPERTY(BlueprintAssignable)
    FOnAttributeChangedSignature OnHealthChanged;

    UPROPERTY(BlueprintAssignable)
    FOnAttributeChangedSignature OnMaxHealthChanged;

    void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

    UPROPERTY(BlueprintReadOnly, Category = "Combat")
    bool bHitReacting = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float BaseWalkSpeed = 250.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    float LifeSpan = 5.f;

  protected:
    virtual void BeginPlay() override;
    virtual void InitAbilityActorInfo() override;
    virtual void InitializeDefaultAttributes() const override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
    int32 Level = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
    ECharacterClass CharacterClass = ECharacterClass::ECC_Warrior;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UWidgetComponent> HealthBar;
};
