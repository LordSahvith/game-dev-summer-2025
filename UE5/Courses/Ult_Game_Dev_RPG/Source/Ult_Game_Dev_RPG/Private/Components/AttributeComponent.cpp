#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UAttributeComponent::TickComponent(float DeltaTime,
                                        ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/******************
 * DAMAGE / DEATH *
 ******************/

void UAttributeComponent::RecieveDamage(float Damage)
{
    Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

bool UAttributeComponent::IsAlive()
{
    return Health > 0.f;
}

/***********
 * GETTERS *
 ***********/

float UAttributeComponent::GetHealthPercent()
{
    return Health / MaxHealth;
}

float UAttributeComponent::GetMaxWalkSpeed()
{
    return MaxWalkSpeed;
}

float UAttributeComponent::GetMaxRunSpeed()
{
    return MaxRunSpeed;
}