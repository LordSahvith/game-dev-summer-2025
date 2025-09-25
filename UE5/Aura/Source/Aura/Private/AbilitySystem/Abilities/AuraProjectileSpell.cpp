// Copyright Lord Savith
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
    const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();

    if (!bIsServer) { return; }

    if (ICombatInterface* CombatInteface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
    {
        // Calculate Rotation Direction of the Target
        const FVector SocketLocation = CombatInteface->GetCombatSocketLocation();
        FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
        Rotation.Pitch = 0.f;

        // Rotate to Face Target
        FTransform SpawnTransform;
        SpawnTransform.SetLocation(SocketLocation);
        SpawnTransform.SetRotation(Rotation.Quaternion());

        // Launch Projectile
        AAuraProjectile* Projectile =
            GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,
                                                            SpawnTransform,
                                                            GetOwningActorFromActorInfo(),
                                                            Cast<APawn>(GetOwningActorFromActorInfo()),
                                                            ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

        // Damage: Use Ability System
        const UAbilitySystemComponent* SourceASC =
            UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
        const FGameplayEffectSpecHandle SpecHandle =
            SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

        FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, 50.f);

        Projectile->DamageEffectSpecHandle = SpecHandle;

        // Clean Up Spawn
        Projectile->FinishSpawning(SpawnTransform);
    }
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
