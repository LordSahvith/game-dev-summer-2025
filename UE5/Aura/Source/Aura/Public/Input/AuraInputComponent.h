// Copyright Lord Savith
#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputConfig.h"
#include "AuraInputComponent.generated.h"

UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
    GENERATED_BODY()

  public:
    template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
    void BindAbilityActions(const UAuraInputConfig* InputConfig,
                            UserClass* Object,
                            PressedFuncType PressedFunc,
                            ReleasedFuncType ReleasedFunc,
                            HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
inline void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig,
                                                    UserClass* Object,
                                                    PressedFuncType PressedFunc,
                                                    ReleasedFuncType ReleasedFunc,
                                                    HeldFuncType HeldFunc)
{
    check(InputConfig);

    for (const FAuraInputAction& Action : InputConfig->AbilityInputActions)
    {
        if (Action.InputAction && Action.InputTag.IsValid())
        {
            if (PressedFunc)
            {
                // Params after Function are passed to Function
                BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
            }

            if (ReleasedFunc)
            {
                // Params after Function are passed to Function
                BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
            }

            if (HeldFunc)
            {
                // Params after Function are passed to Function
                BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
            }
        }
    }
}
