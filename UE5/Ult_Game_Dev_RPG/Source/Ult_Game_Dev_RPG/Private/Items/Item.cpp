// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Item.h"
#include "Ult_Game_Dev_RPG/DebugMacros.h"

AItem::AItem()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    RunningTime += DeltaTime;

    float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant);

    AddActorWorldOffset(FVector(0.f, 0.f, DeltaZ));

    DRAW_SPHERE_Singleframe(GetActorLocation());
    DRAW_VECTOR_Singleframe(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);
}
