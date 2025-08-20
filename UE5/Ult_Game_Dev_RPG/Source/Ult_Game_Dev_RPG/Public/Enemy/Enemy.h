#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UParticleSystem;
class UHealthBarComponent;
class AAIController;
class UPawnSensingComponent;

UCLASS()
class ULT_GAME_DEV_RPG_API AEnemy : public ABaseCharacter
{
    GENERATED_BODY()

  public:
    AEnemy();
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Interface
    virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

    virtual float TakeDamage(float DamageAmount,
                             struct FDamageEvent const& DamageEvent,
                             class AController* EventInstigator,
                             AActor* DamageCauser) override;

    virtual void Destroyed() override;

  protected:
    virtual void BeginPlay() override;
    virtual void Die() override;
    bool InTargetRange(AActor* Target, double Radius);
    void MoveToTarget(AActor* Target);
    AActor* ChoosePatrolTarget();

    UFUNCTION()
    void PawnSeen(APawn* SeenPawn);

  private:
    /**************
     * COMPONENTS *
     **************/
    UPROPERTY(VisibleAnywhere)
    UHealthBarComponent* HealthBarWidget;

    UPROPERTY(VisibleAnywhere)
    UPawnSensingComponent* PawnSensing;

    /**********
     * STATES *
     **********/
    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    EDeathPose DeathPose = EDeathPose::EDP_Alive;

    EEnemyState EnemyState = EEnemyState::EES_Patrolling;
    EDeathPose GetDeathPose(int32 PoseType);

    /**********
     * COMBAT *
     **********/
    UPROPERTY(EditAnywhere)
    TSubclassOf<AWeapon> WeaponClass;

    /******************
     * DAMAGE / DEATH *
     ******************/
    UPROPERTY()
    AActor* CombatTarget;

    UPROPERTY(EditAnywhere)
    double CombatRadius = 500.f;

    UPROPERTY(EditAnywhere)
    double AttackRadius = 150.f;

    /***********************
     * WEAPON SOCKET NAMES *
     ***********************/
    UPROPERTY(EditAnywhere, Category = "Weapon")
    FName EquippedSocket = FName("RightSocket");

    /***************************
     * PATROLLING / NAVIGATION *
     ***************************/
    UPROPERTY()
    AAIController* EnemyController;

    // Current Target
    UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
    AActor* PatrolTarget;

    UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
    TArray<AActor*> PatrolTargets;

    UPROPERTY(EditAnywhere)
    double PatrolRadius = 200.f;

    FTimerHandle PatrolTimer;
    void PatrolTimerFinished();

    UPROPERTY(EditAnywhere, Category = "AI Navigation")
    float WaitMin = 5.f;

    UPROPERTY(EditAnywhere, Category = "AI Navigation")
    float WaitMax = 10.f;

    /***********
     * HELPERS *
     ***********/
    void CheckCombatTarget();
    void CheckPatrolTarget();
};
