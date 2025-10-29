// Copyright Lord Savith
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActorHoverInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActorHoverInterface : public UInterface
{
    GENERATED_BODY()
};

class AURA_API IActorHoverInterface
{
    GENERATED_BODY()

  public:
    virtual void HighlightActor() = 0;
    virtual void UnHighlightActor() = 0;
};
