// Copyright Lord Savith
#pragma once

#include "CoreMinimal.h"
#include "Interaction/ActorHoverInterface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UActorHoverInterface
{
    GENERATED_BODY()
};

class AURA_API IEnemyInterface : public IActorHoverInterface
{
    GENERATED_BODY()
};
