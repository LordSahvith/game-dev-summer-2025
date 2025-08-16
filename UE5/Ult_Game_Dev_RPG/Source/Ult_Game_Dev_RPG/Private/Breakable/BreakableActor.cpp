#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Treasure.h"
#include "Components/CapsuleComponent.h"

ABreakableActor::ABreakableActor()
{
    PrimaryActorTick.bCanEverTick = false;

    GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
    SetRootComponent(GeometryCollection);
    GeometryCollection->SetGenerateOverlapEvents(true);
    GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

    Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
    Capsule->SetupAttachment(GetRootComponent());
    Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void ABreakableActor::BeginPlay()
{
    Super::BeginPlay();
}

void ABreakableActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
    // 1/10 times it won't spawn treasure
    bool bShouldSpawnTreasure = FMath::RandRange(0, TreasureSpawnAmount) == TreasureSpawnAmount;
    if (bShouldSpawnTreasure || bHasAlreadySpawnedTreasure) return;

    if (GetWorld() && TreasureClasses.Num() > 0)
    {
        int32 TreasureType = FMath::RandRange(0, TreasureClasses.Num() - 1);
        FVector Location = GetActorLocation();
        Location.Z += 75.f;
        GetWorld()->SpawnActor<ATreasure>(TreasureClasses[TreasureType], Location, GetActorRotation());
        bHasAlreadySpawnedTreasure = true;
    }
}
