// Copyright Lord Savith
#include "Actor/AuraEffectActor.h"
#include "Aura/Aura.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Pickup Mesh");
    Mesh->SetupAttachment(GetRootComponent());
}

void AAuraEffectActor::BeginPlay() { Super::BeginPlay(); }

void AAuraEffectActor::HighlightActor()
{
    Mesh->SetRenderCustomDepth(true);
    Mesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEffectActor::UnHighlightActor() { Mesh->SetRenderCustomDepth(false); }

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

    if (TargetASC == nullptr) { return; }

    check(GameplayEffectClass);

    FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
    EffectContextHandle.AddSourceObject(this);
    const FGameplayEffectSpecHandle EffectSpecHandle =
        TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);

    const FActiveGameplayEffectHandle ActiveEffectHandle =
        TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

    const bool bIsInfinite =
        EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

    if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::EERP_RemoveOnEndOverlap)
    {
        ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
    }
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
    if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::EEAP_ApplyOnOverlap)
    {
        ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
    }

    if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::EEAP_ApplyOnOverlap)
    {
        ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
    }

    if (InfinteEffectApplicationPolicy == EEffectApplicationPolicy::EEAP_ApplyOnOverlap)
    {
        ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
    }
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
    if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::EEAP_ApplyOnEndOverlap)
    {
        ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
    }

    if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::EEAP_ApplyOnEndOverlap)
    {
        ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
    }

    if (InfinteEffectApplicationPolicy == EEffectApplicationPolicy::EEAP_ApplyOnEndOverlap)
    {
        ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
    }

    if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::EERP_RemoveOnEndOverlap)
    {
        UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

        if (!IsValid(TargetASC)) return;

        TArray<FActiveGameplayEffectHandle> HandlesToRemove;

        for (auto HandlePair : ActiveEffectHandles)
        {
            if (TargetASC == HandlePair.Value)
            {
                TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
                HandlesToRemove.Add(HandlePair.Key);
            }
        }

        for (auto Handle : HandlesToRemove)
        {
            ActiveEffectHandles.FindAndRemoveChecked(Handle);
        }
    }
}
