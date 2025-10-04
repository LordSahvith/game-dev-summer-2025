// Copyright Lord Savith
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AuraAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class AURA_API AAuraAIController : public AAIController
{
    GENERATED_BODY()

  public:
    AAuraAIController();

  protected:
    UPROPERTY()
    TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

    UPROPERTY()
    TObjectPtr<UBlackboardComponent> BlackboardComponent;
};
