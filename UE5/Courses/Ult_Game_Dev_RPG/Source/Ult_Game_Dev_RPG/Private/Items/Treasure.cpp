#include "Items/Treasure.h"
#include "Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                     AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp,
                                     int32 OtherBodyIndex,
                                     bool bFromSweep,
                                     const FHitResult& SweepResult)
{
    AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);

    if (MainCharacter && PickupSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
        Destroy();
    }
}