// Copyright Lord Savith
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AAuraAIController::AAuraAIController()
{
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("Behavior Tree Component");
    check(BehaviorTreeComponent);
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("Blackboard Component");
    check(BlackboardComponent);
}
