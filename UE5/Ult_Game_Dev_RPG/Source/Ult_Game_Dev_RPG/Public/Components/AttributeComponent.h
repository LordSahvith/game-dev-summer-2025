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

    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
    float MaxWalkSpeed = 125.f;

    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
    float MaxRunSpeed = 300.f;

  public:
    /******************
     * DAMAGE / DEATH *
     ******************/
    void RecieveDamage(float Damage);
    bool IsAlive();

    /***********
     * GETTERS *
     ***********/
    float GetHealthPercent();
    float GetMaxWalkSpeed();
    float GetMaxRunSpeed();
};
