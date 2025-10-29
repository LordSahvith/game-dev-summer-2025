// Copyright Lord Savith
#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotfound) const
{
    for (const FAuraAttributeInfo& Info : AttributeInformation)
    {
        if (Info.AttributeTag.MatchesTagExact(AttributeTag)) { return Info; }
    }

    if (bLogNotfound)
    {
        UE_LOG(LogTemp,
               Error,
               TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."),
               *AttributeTag.ToString(),
               *GetNameSafe(this));
    }

    return FAuraAttributeInfo();
}
