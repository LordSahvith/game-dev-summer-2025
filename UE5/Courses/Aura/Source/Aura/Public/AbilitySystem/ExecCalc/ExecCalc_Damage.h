// Copyright Lord Savith
#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

class UCharacterClassInfo;
class ICombatInterface;

UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
    GENERATED_BODY()

  public:
    UExecCalc_Damage();

    virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                        FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

  private:
    float GetAttributeMagnitude(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                const FGameplayEffectAttributeCaptureDefinition& AttributeDef,
                                FAggregatorEvaluateParameters& EvaluationParameters,
                                float& OutAttributeValue) const;
    float GetCurveCoefficient(const UCharacterClassInfo* CharacterClassInfo,
                              const FName CurveName,
                              const ICombatInterface* CombatInterface) const;
};
