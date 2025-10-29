#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"

class USoundBase;

UCLASS()
class ULT_GAME_DEV_RPG_API ATreasure : public AItem
{
    GENERATED_BODY()

  protected:
    virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                      AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp,
                                      int32 OtherBodyIndex,
                                      bool bFromSweep,
                                      const FHitResult& SweepResult) override;

  private:
    /**
     * Sound Effects
     */
    UPROPERTY(EditAnywhere, Category = "Sound Effects")
    USoundBase* PickupSound;

    UPROPERTY(EditAnywhere, Category = "Treasure")
    int32 Gold;
};
