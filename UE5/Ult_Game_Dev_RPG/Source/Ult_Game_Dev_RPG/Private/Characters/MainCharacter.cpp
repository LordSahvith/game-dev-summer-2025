#include "Characters/MainCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"

AMainCharacter::AMainCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 450.f, 0.f);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(GetRootComponent());
    SpringArm->TargetArmLength = 300.f;
    SpringArm->bUsePawnControlRotation = true;

    ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
    ViewCamera->SetupAttachment(SpringArm);

    Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
    Hair->SetupAttachment(GetMesh());
    Hair->AttachmentName = FString("head");

    Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
    Eyebrows->SetupAttachment(GetMesh());
    Eyebrows->AttachmentName = FString("head");
}

void AMainCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(CharacterMappingContext, 0);
        }
    }

    AnimInstance = GetMesh()->GetAnimInstance();
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
    if (ActionState != EActionState::EAS_Unoccupied)
        return;

    const FVector2D MovementVector = Value.Get<FVector2D>();

    if (GetController())
    {
        const FRotator Rotation = GetController()->GetControlRotation();
        const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookVector = Value.Get<FVector2D>();

    if (GetController())
    {
        AddControllerYawInput(LookVector.X);
        AddControllerPitchInput(LookVector.Y);
    }
}

void AMainCharacter::Jump()
{
    Super::Jump();

    // UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

    // // Which Type of Jump
    // if (AnimInstance && JumpMontage && !GetCharacterMovement()->IsFalling())
    // {
    //     FString JumpType = "";

    //     switch (CharacterState)
    //     {
    //         case ECharacterState::ECS_EquippedOneHandedWeapon:
    //             JumpType = "EquippedOneHanded";
    //             break;
    //         case ECharacterState::ECS_EquippedTwoHandedWeapon:
    //             JumpType = "EquippedTwoHanded";
    //             break;
    //         case ECharacterState::ECS_Unequipped:
    //         default:
    //             break;
    //     }

    //     FString JumpName = "Jump";
    //     JumpName.Append(JumpType);

    //     AnimInstance->Montage_Play(JumpMontage, 2.f);
    //     AnimInstance->Montage_JumpToSection(FName(JumpName), JumpMontage);
    // }
}

void AMainCharacter::Equip(const FInputActionValue& Value)
{
    AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);

    if (OverlappingWeapon)
    {
        OverlappingWeapon->Equip(GetMesh(), EquippedSocket);
        CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
        OverlappingItem = nullptr;
        EquippedWeapon = OverlappingWeapon;
    }
    else
    {
        if (CanSheathWeapon())
        {
            PlayMontageEquip(SheathWeaponName);
            CharacterState = ECharacterState::ECS_Unequipped;
            ActionState = EActionState::EAS_Sheathing;
        }
        else if (CanDrawWeapon())
        {
            PlayMontageEquip(DrawWeaponName);
            CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
            ActionState = EActionState::EAS_Sheathing;
        }
    }
}

void AMainCharacter::Attack(const FName& AttackType)
{
    switch (CharacterState)
    {
        case ECharacterState::ECS_EquippedOneHandedWeapon:
            ActionState = EActionState::EAS_Attacking;
            PlayMontageOneHandedAttack(AttackType);
            break;
    }
}

void AMainCharacter::AttackBasic(const FInputActionValue& Value)
{

    // ActionState gets reset from Animation Blueprint
    // calling: AMainCharacter::AttackEnd()
    if (CanAttack())
    {
        if (GEngine)
        {
            FString Message = FString("Combo - 1");
            GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Cyan, Message);
        }
        Attack(BasicAttack);
    }
}

void AMainCharacter::AttackThreePartCombo(const FInputActionValue& Value)
{
    if (GEngine)
    {
        FString Message = FString("Combo - Finished");
        GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Cyan, Message);
    }
}

void AMainCharacter::AttackCircle(const FInputActionValue& Value)
{

    if (CanAttack())
    {
        if (GEngine)
        {
            FString Message = FString("Combo - 2");
            GEngine->AddOnScreenDebugMessage(3, 2.f, FColor::Cyan, Message);
        }
        Attack(CircleAttack);
    }
}

void AMainCharacter::AttackHeavy(const FInputActionValue& Value)
{

    if (CanAttack())
    {
        if (GEngine)
        {
            FString Message = FString("Combo - 3");
            GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Cyan, Message);
        }
        Attack(HeavyAttack);
    }
}

void AMainCharacter::PlayMontageOneHandedAttack(const FName AttackName)
{
    if (AnimInstance && OneHandedAttackMontage)
    {
        AnimInstance->Montage_Play(OneHandedAttackMontage);
        AnimInstance->Montage_JumpToSection(AttackName, OneHandedAttackMontage);
    }
}

void AMainCharacter::PlayMontageEquip(FName SectionName)
{
    if (AnimInstance && EquipMontage)
    {
        AnimInstance->Montage_Play(EquipMontage);
        AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
    }
}

/**
 * Called from Animation Blueprint's EventGraph
 */
void AMainCharacter::AttackEnd()
{
    ActionState = EActionState::EAS_Unoccupied;
}

/**
 * Called from Animation Blueprint's EventGraph
 */
void AMainCharacter::SheathEnd()
{
    ActionState = EActionState::EAS_Unoccupied;
}

void AMainCharacter::SheathWeapon()
{
    if (EquippedWeapon)
    {
        EquippedWeapon->AttachMeshToSocket(GetMesh(), SheathedSocket);
    }
}

void AMainCharacter::DrawWeapon()
{
    if (EquippedWeapon)
    {
        EquippedWeapon->AttachMeshToSocket(GetMesh(), EquippedSocket);
    }
}

bool AMainCharacter::CanAttack()
{
    return ActionState == EActionState::EAS_Unoccupied;
}

bool AMainCharacter::CanSheathWeapon()
{
    return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped &&
           EquippedWeapon;
}

bool AMainCharacter::CanDrawWeapon()
{
    return ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped &&
           EquippedWeapon;
}

void AMainCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainCharacter::Jump);
        EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AMainCharacter::Equip);
        EnhancedInputComponent->BindAction(AttackBasicAction, ETriggerEvent::Triggered, this,
                                           &AMainCharacter::AttackBasic);
        EnhancedInputComponent->BindAction(AttackThreePartComboAction, ETriggerEvent::Completed, this,
                                           &AMainCharacter::AttackThreePartCombo);
        EnhancedInputComponent->BindAction(AttackCircleAction, ETriggerEvent::Triggered, this,
                                           &AMainCharacter::AttackCircle);
        EnhancedInputComponent->BindAction(AttackHeavyAction, ETriggerEvent::Triggered, this,
                                           &AMainCharacter::AttackHeavy);
    }
}
