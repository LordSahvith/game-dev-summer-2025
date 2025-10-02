// Copyright Lord Savith
#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
    /**
     * PRIMARY ATTRIBUTES
     */
    GameplayTags.Attributes_Primary_Strength =
        SetGameplayTag(FName("Attributes.Primary.Strength"), FString("Increases Physical Damage"));
    GameplayTags.Attributes_Primary_Intelligence =
        SetGameplayTag(FName("Attributes.Primary.Intelligence"), FString("Increases Magical Damage"));
    GameplayTags.Attributes_Primary_Resilience =
        SetGameplayTag(FName("Attributes.Primary.Resilience"), FString("Increases Armor & Armor Penetration"));
    GameplayTags.Attributes_Primary_Vigor =
        SetGameplayTag(FName("Attributes.Primary.Vigor"), FString("Increases Health"));

    /**
     * SECONDARY ATTRIBUTES
     */
    GameplayTags.Attributes_Secondary_Armor =
        SetGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces Damage Taken, improves Block Chance."));
    GameplayTags.Attributes_Secondary_ArmorPenetration =
        SetGameplayTag(FName("Attributes.Secondary.ArmorPenetration"),
                       FString("Ignored percentage of Enemy Armor, increases Critical Hit Chance"));
    GameplayTags.Attributes_Secondary_BlockChance =
        SetGameplayTag(FName("Attributes.Secondary.BlockChance"), FString("Chance to cut incoming damage in half"));
    GameplayTags.Attributes_Secondary_CriticalHitChance = SetGameplayTag(
        FName("Attributes.Secondary.CriticalHitChance"), FString("Chance to Double Damage plus Critical Hit Bonus"));
    GameplayTags.Attributes_Secondary_CriticalHitDamage = SetGameplayTag(
        FName("Attributes.Secondary.CriticalHitDamage"), FString("Bonus Damage added when a Critical Hit is scored"));
    GameplayTags.Attributes_Secondary_CriticalHitResistance = SetGameplayTag(
        FName("Attributes.Secondary.CriticalHitResistance"), FString("Reduces Critical Hit Chance of Enemies"));
    GameplayTags.Attributes_Secondary_HealthRegeneration = SetGameplayTag(
        FName("Attributes.Secondary.HealthRegeneration"), FString("Amount of Health regenerated every 1 second"));
    GameplayTags.Attributes_Secondary_ManaRegeneration = SetGameplayTag(
        FName("Attributes.Secondary.ManaRegeneration"), FString("Amount of Mana regenerated every 1 second"));
    GameplayTags.Attributes_Secondary_MaxHealth =
        SetGameplayTag(FName("Attributes.Secondary.MaxHealth"), FString("Increases Health"));
    GameplayTags.Attributes_Secondary_MaxMana =
        SetGameplayTag(FName("Attributes.Secondary.MaxMana"), FString("Increases Mana"));

    /**
     * INPUT TAGS
     */
    GameplayTags.InputTag_LMB = SetGameplayTag(FName("InputTag.LMB"), FString("Input Tag for Left Mouse Button"));
    GameplayTags.InputTag_RMB = SetGameplayTag(FName("InputTag.RMB"), FString("Input Tag for Right Mouse Button"));
    GameplayTags.InputTag_1 = SetGameplayTag(FName("InputTag.1"), FString("Input Tag for 1 key"));
    GameplayTags.InputTag_2 = SetGameplayTag(FName("InputTag.2"), FString("Input Tag for 2 key"));
    GameplayTags.InputTag_3 = SetGameplayTag(FName("InputTag.3"), FString("Input Tag for 3 key"));
    GameplayTags.InputTag_4 = SetGameplayTag(FName("InputTag.4"), FString("Input Tag for 4 key"));

    /**
     * COMBAT
     */
    GameplayTags.Damage = SetGameplayTag(FName("Damage"), FString("Damage"));
    GameplayTags.Damage_Fire = SetGameplayTag(FName("Damage.Fire"), FString("Fire Damage Type"));

    GameplayTags.DamageTypes.Add(GameplayTags.Damage_Fire);

    GameplayTags.Effects_HitReact = SetGameplayTag(FName("Effects.HitReact"), FString("Tag granted when Hit Reacting"));
}

FGameplayTag FAuraGameplayTags::SetGameplayTag(FName TagName, FString Description)
{
    return UGameplayTagsManager::Get().AddNativeGameplayTag(TagName, Description);
}