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
    virtual float TakeDamage(float DamageAmount,
                             struct FDamageEvent const& DamageEvent,
                             class AController* EventInstigator,
                             AActor* DamageCauser) override;
    virtual void Destroyed() override;

    /**
     * @interface: IHitInterface
     */
    virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

  protected:
    virtual void BeginPlay() override;
    virtual void Die() override;

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
    EDeathPose DeathPose;

    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    EEnemyState EnemyState = EEnemyState::EES_Patrolling;

    /***************************
     * COMBAT - DAMAGE / DEATH *
     ***************************/
    virtual bool CanAttack() override;
    virtual void HandleDamage(float DamageAmount);
    void AttackTarget();
    void StartAttackTimer();
    void ClearAttackTimer();

    FTimerHandle AttackTimer;

    UPROPERTY()
    AActor* CombatTarget;

    UPROPERTY(EditAnywhere)
    double CombatRadius = 1000.f;

    UPROPERTY(EditAnywhere)
    double AttackRadius = 150.f;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AWeapon> WeaponClass;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float AttackWaitMin = 0.5f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float AttackWaitMax = 1.f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float DeathLifeSpan = 8.f;

    /**********
     * WEAPON *
     **********/
    void SpawnDefaultWeapon();

    UPROPERTY(EditAnywhere, Category = "Weapon")
    FName EquippedSocket = FName("RightSocket");

    /***************************
     * PATROLLING / NAVIGATION *
     ***************************/
    void PatrolTimerFinished();
    void StartPatrolTimer();
    void ClearPatrolTimer();

    FTimerHandle PatrolTimer;

    UPROPERTY()
    AAIController* EnemyController;

    // Current Target
    UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
    AActor* PatrolTarget;

    UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
    TArray<AActor*> PatrolTargets;

    UPROPERTY(EditAnywhere, Category = "AI Navigation")
    double PatrolRadius = 200.f;

    UPROPERTY(EditAnywhere, Category = "AI Navigation")
    float PatrolWaitMin = 5.f;

    UPROPERTY(EditAnywhere, Category = "AI Navigation")
    float PatrolWaitMax = 10.f;

    /**********************
     * ANIMATION MONTAGES *
     **********************/
    virtual int32 PlayMontageDeath() override;

    /*********************************
     * ANIMATION BLUEPRINT NOTIFIERS *
     *********************************/
    virtual void AttackEnd() override;

    /***************
     * AI BEHAVIOR *
     ***************/
    void InitializeEnemy();
    void ToggleHealthBar(bool bShouldShow);
    void HideHealthBar();
    void ShowHealthBar();
    void LoseInterest();
    void StartPatrolling();
    void ChaseTarget();
    bool IsOutsideCombatRadius();
    bool IsOutsideAttackRadius();
    bool IsInsideAttackRadius();
    bool IsDead();
    bool IsPatrolling();
    bool IsChasing();
    bool IsAttacking();
    bool IsEngaged();
    void CheckCombatTarget();
    void CheckPatrolTarget();
    bool InTargetRange(AActor* Target, double Radius);
    void MoveToTarget(AActor* Target);
    AActor* ChoosePatrolTarget();

    UFUNCTION()
    void PawnSeen(APawn* SeenPawn);
};
