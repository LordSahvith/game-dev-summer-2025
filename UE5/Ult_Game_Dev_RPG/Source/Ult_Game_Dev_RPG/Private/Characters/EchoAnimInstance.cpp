#include "Characters/EchoAnimInstance.h"
#include "Characters/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEchoAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());

    if (MainCharacter)
    {
        CharacterMovement = MainCharacter->GetCharacterMovement();
    }
}

void UEchoAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (MainCharacter)
    {
        GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);
    }
}
