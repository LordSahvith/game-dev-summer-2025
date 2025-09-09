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
    FAuraAttributeInfo Info =
        AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength);
    Info.AttributeValue = AS->GetStrength();

    AttributeInfoDelegate.Broadcast(Info);
}
