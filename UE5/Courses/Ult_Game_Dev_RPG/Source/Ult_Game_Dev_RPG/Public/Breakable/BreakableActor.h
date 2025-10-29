#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;
class USoundBase;
class ATreasure;
class UCapsuleComponent;

UCLASS()
class ULT_GAME_DEV_RPG_API ABreakableActor : public AActor, public IHitInterface
{
    GENERATED_BODY()

  public:
    ABreakableActor();
    virtual void Tick(float DeltaTime) override;

    virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UCapsuleComponent* Capsule;

  private:
    UPROPERTY(VisibleAnywhere)
    UGeometryCollectionComponent* GeometryCollection;

    UPROPERTY(EditAnywhere, Category = "Breakable Properties")
    TArray<TSubclassOf<ATreasure>> TreasureClasses;

    UPROPERTY(EditAnywhere, Category = "Treasure")
    int32 TreasureSpawnAmount = 10;

    bool bHasAlreadySpawnedTreasure = false;
};
