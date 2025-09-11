// Copyright Lord Savith
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    const bool bIsServer = HasAuthority(&ActivationInfo);

    if (!bIsServer) { return; }

    if (ICombatInterface* CombatInteface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
    {
        const FVector SocketLocation = CombatInteface->GetCombatSocketLocation();

        FTransform SpawnTransform;
        SpawnTransform.SetLocation(SocketLocation);

        // TODO: set the projectile rotation

        AAuraProjectile* Projectile =
            GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,
                                                            SpawnTransform,
                                                            GetOwningActorFromActorInfo(),
                                                            Cast<APawn>(GetOwningActorFromActorInfo()),
                                                            ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

        // TODO: give the projectile a gameplay effect spec for causing damage
        Projectile->FinishSpawning(SpawnTransform);
    }
}
