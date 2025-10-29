// Copyright Lord Savith
#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
    return CharacterClassInfo.FindChecked(CharacterClass);
}