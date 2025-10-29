// character
#include "Characters/MainCharacter.h"
#include "Items/Item.h"
#include "GroomComponent.h"
#include "Components/AttributeComponent.h"

// Input
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// engine components
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"

// weapon
#include "Items/Weapons/Weapon.h"

AMainCharacter::AMainCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 450.f, 0.f);

    GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
    GetMesh()->SetGenerateOverlapEvents(true);

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

/*****************************************
 * INHERITED OVERRIDES OF BASIC GAMEPLAY *
 *****************************************/
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

    Tags.Add(GetEngageableTagName());
}

/*********
 * INPUT *
 *********/
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainCharacter::Jump);
        EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AMainCharacter::Equip);
        EnhancedInputComponent->BindAction(
            AttackLightAction, ETriggerEvent::Triggered, this, &AMainCharacter::AttackLight);
        EnhancedInputComponent->BindAction(
            AttackMediumAction, ETriggerEvent::Triggered, this, &AMainCharacter::AttackMedium);
        EnhancedInputComponent->BindAction(
            AttackHeavyAction, ETriggerEvent::Triggered, this, &AMainCharacter::AttackHeavy);
    }
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
    if (ActionState != EActionState::EAS_Unoccupied) return;

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
}

void AMainCharacter::Equip(const FInputActionValue& Value)
{
    AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);

    if (OverlappingWeapon)
    {
        EquipWeapon(OverlappingWeapon);
    }
    else
    {
        if (CanSheathWeapon())
        {
            SheathWeapon();
        }
        else if (CanDrawWeapon())
        {
            DrawWeapon();
        }
    }
}

/***************************
 * COMBAT - DAMAGE / DEATH *
 ***************************/
// Interface
void AMainCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
    PlayHitSound(ImpactPoint);
    SpawnHitParticles(ImpactPoint);
}

void AMainCharacter::Attack(const FName& AttackType)
{
    switch (CharacterState)
    {
        case ECharacterState::ECS_EquippedOneHandedWeapon:
            ActionState = EActionState::EAS_Attacking;
            Super::Attack(AttackType);
            break;
    }
}

void AMainCharacter::AttackLight(const FInputActionValue& Value)
{
    if (CanAttack())
    {
        Attack(LightAttack);
    }
}

void AMainCharacter::AttackMedium(const FInputActionValue& Value)
{

    if (CanAttack())
    {
        Attack(MediumAttack);
    }
}

void AMainCharacter::AttackHeavy(const FInputActionValue& Value)
{

    if (CanAttack())
    {
        Attack(HeavyAttack);
    }
}

/**************************************************
 * COMBAT HELPERS - ANIMATION BLUEPRINT NOTIFIERS *
 **************************************************/
void AMainCharacter::AttackEnd()
{
    ActionState = EActionState::EAS_Unoccupied;
}

void AMainCharacter::SheathEnd()
{
    ActionState = EActionState::EAS_Unoccupied;
}

void AMainCharacter::AttachWeaponToBack()
{
    if (EquippedWeapon)
    {
        EquippedWeapon->AttachMeshToSocket(GetMesh(), SheathedSocket);
    }
}

void AMainCharacter::AttachWeaponToHand()
{
    if (EquippedWeapon)
    {
        EquippedWeapon->AttachMeshToSocket(GetMesh(), EquippedSocket);
    }
}

/*****************************
 * COMBAT HELPERS - INTERNAL *
 *****************************/
bool AMainCharacter::CanAttack()
{
    // ActionState gets reset from Animation Blueprint
    // calling: AMainCharacter::AttackEnd()
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

void AMainCharacter::EquipWeapon(AWeapon* Weapon)
{
    Weapon->Equip(GetMesh(), EquippedSocket, this, this);
    CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
    OverlappingItem = nullptr;
    EquippedWeapon = Weapon;
}

void AMainCharacter::SheathWeapon()
{
    PlayMontageSection(SheathWeaponName, EquipMontage);
    CharacterState = ECharacterState::ECS_Unequipped;
    ActionState = EActionState::EAS_Sheathing;
}

void AMainCharacter::DrawWeapon()
{
    PlayMontageSection(DrawWeaponName, EquipMontage);
    CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
    ActionState = EActionState::EAS_Sheathing;
}
