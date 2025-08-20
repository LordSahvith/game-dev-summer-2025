#include "Items/Item.h"
#include "Components/SphereComponent.h"
#include "Characters/MainCharacter.h"
#include "NiagaraComponent.h"

AItem::AItem()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
    ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SetRootComponent(ItemMesh);

    Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    Sphere->SetupAttachment(GetRootComponent());

    EmbersEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers Effect"));
    EmbersEffect->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
    Super::BeginPlay();

    // Bind Delegates
    Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
    Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    RunningTime += DeltaTime;

    if (ItemState == EItemState::EIS_Hovering)
    {
        AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
        AddActorWorldRotation(FQuat(FRotator(0.f, HoverSpinSpeed, 0.f)));
    }
}

float AItem::TransformedSin()
{
    return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
    return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                 AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp,
                                 int32 OtherBodyIndex,
                                 bool bFromSweep,
                                 const FHitResult& SweepResult)
{
    AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);

    if (MainCharacter)
    {
        MainCharacter->SetOverlappingItem(this);
    }
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
                               AActor* OtherActor,
                               UPrimitiveComponent* OtherComp,
                               int32 OtherBodyIndex)
{
    AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);

    if (MainCharacter)
    {
        MainCharacter->SetOverlappingItem(nullptr);
    }
}
