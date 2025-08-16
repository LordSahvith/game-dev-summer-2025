#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ULT_GAME_DEV_RPG_API UAttributeComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    UAttributeComponent();
    virtual void TickComponent(float DeltaTime,
                               ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

  protected:
    virtual void BeginPlay() override;

  private:
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
    float Health = 100.f; // current health

    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
    float MaxHealth = 100.f;

  public:
    void RecieveDamage(float Damage);
    float GetHealthPercent();
    bool IsAlive();
};
