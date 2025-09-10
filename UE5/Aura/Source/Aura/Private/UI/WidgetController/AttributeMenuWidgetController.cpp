// Copyright Lord Savith
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies() {}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
    check(AttributeInfo);

    UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

    for (auto& Pair : AS->TagsToAttributes)
    {
        FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
        Info.AttributeValue = Pair.Value().GetNumericValue(AS);
        AttributeInfoDelegate.Broadcast(Info);
    }
}
