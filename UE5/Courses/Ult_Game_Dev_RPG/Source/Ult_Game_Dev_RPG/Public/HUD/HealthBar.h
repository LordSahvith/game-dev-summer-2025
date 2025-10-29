#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class UProgressBar;

UCLASS()
class ULT_GAME_DEV_RPG_API UHealthBar : public UUserWidget
{
    GENERATED_BODY()

  public:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;
};
