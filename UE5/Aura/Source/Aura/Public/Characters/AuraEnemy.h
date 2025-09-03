// Copyright Lord Savith
#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

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

  protected:
    virtual void BeginPlay() override;
    virtual void InitAbilityActorInfo() override;
};
