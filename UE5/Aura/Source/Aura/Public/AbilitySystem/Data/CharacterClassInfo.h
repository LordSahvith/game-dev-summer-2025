// Copyright Lord Savith
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
    ECC_Elementalist UMETA(DisplayName = "Elementalist"),
    ECC_Warrior UMETA(DisplayName = "Warrior"),
    ECC_Ranger UMETA(DisplayName = "Ranger")
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
    TSubclassOf<UGameplayEffect> PrimaryAttributes;
};

UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
    GENERATED_BODY()

  public:
    UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
    TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInfo;

    UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
    TSubclassOf<UGameplayEffect> SecondaryAttributes;

    UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
    TSubclassOf<UGameplayEffect> VitalAttributes;

    FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
};
