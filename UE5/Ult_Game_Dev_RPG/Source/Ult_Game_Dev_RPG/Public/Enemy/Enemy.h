#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

class UAnimMontage;

UCLASS()
class ULT_GAME_DEV_RPG_API AEnemy : public ACharacter, public IHitInterface
{
    GENERATED_BODY()

  public:
    AEnemy();
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void GetHit(const FVector& ImpactPoint) override;

  protected:
    virtual void BeginPlay() override;

  private:
    UAnimInstance* AnimInstance;

    /**
     * Animation Blueprint Notify Names
     */
    const FName ReactFromBack = FName("FromBack");
    const FName ReactFromFront = FName("FromFront");
    const FName ReactFromLeft = FName("FromLeft");
    const FName ReactFromRight = FName("FromRight");

    /**
     * Animation Montages
     */
    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* HitReactMontage;

    /**
     * Play Montage Functions
     */
    void PlayMontageHitReact(const FName& AttackName);

    /**
     * Sounds
     */
    UPROPERTY(EditAnywhere, Category = "Sounds")
    USoundBase* HitSound;

    /**
     * Helpers
     */
    void DirectionalHitReact(const FVector& ImpactPoint);
};
