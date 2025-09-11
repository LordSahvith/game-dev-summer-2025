// Copyright Lord Savith
#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "AuraGameplayTags.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

AAuraPlayerController::AAuraPlayerController()
{
    bReplicates = true;
    Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    CursorTrace();
    AutoRun();
}

void AAuraPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Assert Variable is Set
    check(AuraContext);

    // Get Enhanced Input / Mapping Context
    UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

    if (Subsystem) { Subsystem->AddMappingContext(AuraContext, 0); }

    // Mouse Behavior
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;
    FInputModeGameAndUI InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputModeData.SetHideCursorDuringCapture(false);
    SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

    AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
    AuraInputComponent->BindAbilityActions(InputConfig,
                                           this,
                                           &ThisClass::AbilityInputTagPressed,
                                           &ThisClass::AbilityInputTagReleased,
                                           &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

    const FRotator Rotation = GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
    }
}

void AAuraPlayerController::CursorTrace()
{
    GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);

    if (!CursorHit.bBlockingHit) return;

    LastActor = CurrentActor;
    CurrentActor = CursorHit.GetActor();

    if (LastActor != CurrentActor)
    {
        if (LastActor) { LastActor->UnHighlightActor(); }
        if (CurrentActor) { CurrentActor->HighlightActor(); }
    }
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
    if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
    {
        bTargeting = CurrentActor ? true : false;
        bAutoRunning = false;
    }
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (GetASC() == nullptr) { return; }

    if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
    {
        return GetASC()->AbilityInputTagReleased(InputTag);
    }

    if (bTargeting) { GetASC()->AbilityInputTagReleased(InputTag); }
    else
    {
        const APawn* ControlledPawn = GetPawn();

        if (FollowTime <= ShortPressThreshold && ControlledPawn)
        {
            if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
                    this, ControlledPawn->GetActorLocation(), CachedDestination))
            {
                Spline->ClearSplinePoints();

                for (const FVector& PointLoc : NavPath->PathPoints)
                {
                    Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
                }

                CachedDestination =
                    NavPath->PathPoints.IsEmpty() ? ControlledPawn->GetActorLocation() : NavPath->PathPoints.Last();
                bAutoRunning = true;
            }
        }

        FollowTime = 0.f;
        bTargeting = false;
    }
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if (GetASC() == nullptr) { return; }

    if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
    {
        return GetASC()->AbilityInputTagHeld(InputTag);
    }

    if (bTargeting) { GetASC()->AbilityInputTagHeld(InputTag); }
    else
    {
        FollowTime += GetWorld()->GetDeltaSeconds();

        if (CursorHit.bBlockingHit) { CachedDestination = CursorHit.ImpactPoint; }

        if (APawn* ControlledPawn = GetPawn())
        {
            const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
            ControlledPawn->AddMovementInput(WorldDirection);
        }
    }
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
    if (AuraAbilitySystemComponent == nullptr)
    {
        AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
            UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
    }

    return AuraAbilitySystemComponent;
}

void AAuraPlayerController::AutoRun()
{
    if (!bAutoRunning) { return; }

    if (APawn* ControlledPawn = GetPawn())
    {
        const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(),
                                                                                    ESplineCoordinateSpace::World);
        const FVector Direction =
            Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);

        ControlledPawn->AddMovementInput(Direction);

        const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();

        if (DistanceToDestination <= AutoRunAcceptanceRadius) { bAutoRunning = false; }
    }
}
