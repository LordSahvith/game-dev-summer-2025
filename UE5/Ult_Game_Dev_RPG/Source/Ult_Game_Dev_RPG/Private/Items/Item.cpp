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

    // rate (cm/s) * Deltatime (s/frame) = fixed rate per frame (cm/frame)
    float MovementRate = 50.f * DeltaTime;
    float RotationRate = 45.f * DeltaTime;

    AddActorWorldOffset(FVector(MovementRate, 0.f, 0.f));
    AddActorWorldRotation(FRotator(0.f, RotationRate, 0.f));
    DRAW_SPHERE_Singleframe(GetActorLocation());
    DRAW_VECTOR_Singleframe(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);
}
