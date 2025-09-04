// Copyright Lord Savith
#pragma once

#include "CoreMinimal.h"
#include "Characters/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
    GENERATED_BODY()

  public:
    AAuraCharacter();
    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    /**
     * @interface: ICombatInterface
     */
    virtual int32 GetPlayerLevel() override;

  protected:
  private:
    virtual void InitAbilityActorInfo() override;
};
