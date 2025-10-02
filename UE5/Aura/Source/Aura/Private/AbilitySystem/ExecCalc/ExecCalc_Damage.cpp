// Copyright Lord Savith
#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "GameplayEffectAggregator.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "AuraAbilityTypes.h"

struct AuraDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
    DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

    AuraDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
    }
};

static const AuraDamageStatics& DamageStatics()
{
    static AuraDamageStatics DStatics;

    return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
    RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
    RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
    RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{

    const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

    const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
    const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

    const ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
    const ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);

    if (!CharacterClassInfo) { return; }

    // Get Damage set by caller magnitude
    float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);

    // Attribute Magnitudes
    float TargetArmor = 0.f;
    float SourceArmorPenetration = 0.f;
    float TargetBlockChance = 0.f;
    float SourceCriticalHitChance = 0.f;
    float SourceCriticalHitDamage = 0.f;
    float TargetCriticalHitResistance = 0.f;

    TargetArmor = GetAttributeMagnitude(ExecutionParams, DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
    SourceArmorPenetration = GetAttributeMagnitude(
        ExecutionParams, DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
    TargetBlockChance =
        GetAttributeMagnitude(ExecutionParams, DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
    SourceCriticalHitChance = GetAttributeMagnitude(
        ExecutionParams, DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
    SourceCriticalHitDamage = GetAttributeMagnitude(
        ExecutionParams, DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
    TargetCriticalHitResistance = GetAttributeMagnitude(
        ExecutionParams, DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);

    // Attribute Coefficients
    const float ArmorPenetrationCoefficient =
        GetCurveCoefficient(CharacterClassInfo, FName("ArmorPenetration"), SourceCombatInterface);
    const float EffectiveArmorCoefficient =
        GetCurveCoefficient(CharacterClassInfo, FName("EffectiveArmor"), TargetCombatInterface);
    const float CriticalHitResistanceCoefficient =
        GetCurveCoefficient(CharacterClassInfo, FName("CriticalHitResistance"), TargetCombatInterface);

    // ArmorPenetration ignores a percentage of the Target's Armor
    const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;

    // Critical Hit Resistance reduces Critical Hit Chance by a certain percentage
    const float EffectiveCriticalHitChance =
        SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
    const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;

    // Check if Block was successful
    const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;

    // then update Aura Context Handle with data
    FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
    UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
    UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

    // If Block, Cut Damage in half
    Damage = bBlocked ? Damage / 2.f : Damage;

    // Armor ignores a percentage of incoming damage
    Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

    // Double Damage, plus a bonus if Critical Hit
    Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;

    const FGameplayModifierEvaluatedData EvaluatedData(
        UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
    OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

float UExecCalc_Damage::GetAttributeMagnitude(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              const FGameplayEffectAttributeCaptureDefinition& AttributeDef,
                                              FAggregatorEvaluateParameters& EvaluationParameters,
                                              float& OutAttributeValue) const
{
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttributeDef, EvaluationParameters, OutAttributeValue);
    return FMath::Max<float>(OutAttributeValue, 0.f);
}

float UExecCalc_Damage::GetCurveCoefficient(const UCharacterClassInfo* CharacterClassInfo,
                                            const FName CurveName,
                                            const ICombatInterface* CombatInterface) const
{
    const FRealCurve* AttributeCurve =
        CharacterClassInfo->DamageCalculationCoefficients->FindCurve(CurveName, FString());

    return AttributeCurve->Eval(CombatInterface->GetPlayerLevel());
}
