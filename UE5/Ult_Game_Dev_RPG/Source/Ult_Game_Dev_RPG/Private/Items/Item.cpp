// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Item.h"

// Sets default values
AItem::AItem()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("BeginPlay (Item)"))

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, FString("BeginPlay (Item)"));
    }
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UE_LOG(LogTemp, Warning, TEXT("DeltaTime: %f"), DeltaTime);

    if (GEngine)
    {
        FString name = GetName();
        FString message = FString::Printf(TEXT("Item Name: %s"), *name);
        GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, message);

        UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *name);
    }
}
