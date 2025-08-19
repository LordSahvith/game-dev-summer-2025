#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UParticleSystem;
class UAttributeComponent;
class UHealthBarComponent;
class AAIController;
class UPawnSensingComponent;

UCLASS()
class ULT_GAME_DEV_RPG_API AEnemy : public ACharacter, public IHitInterface
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

  protected:
    virtual void BeginPlay() override;
    void Die();
    bool InTargetRange(AActor* Target, double Radius);
    void MoveToTarget(AActor* Target);
    AActor* ChoosePatrolTarget();

    UFUNCTION()
    void PawnSeen(APawn* SeenPawn);

    /**********
     * STATES *
     **********/
    UPROPERTY(BlueprintReadOnly)
    EDeathPose DeathPose = EDeathPose::EDP_Alive;

  private:
    UAnimInstance* AnimInstance;

    /**************
     * COMPONENTS *
     **************/
    UPROPERTY(VisibleAnywhere)
    UAttributeComponent* Attributes;

    UPROPERTY(VisibleAnywhere)
    UHealthBarComponent* HealthBarWidget;

    UPROPERTY(VisibleAnywhere)
    UPawnSensingComponent* PawnSensing;

    /**********
     * STATES *
     **********/
    EEnemyState EnemyState = EEnemyState::EES_Patrolling;
    EDeathPose GetDeathPose(int32 PoseType);

    /************************************
     * ANIMATION BLUEPRINT NOTIFY NAMES *
     ************************************/
    const FName ReactFromBack = FName("FromBack");
    const FName ReactFromFront = FName("FromFront");
    const FName ReactFromLeft = FName("FromLeft");
    const FName ReactFromRight = FName("FromRight");

    /**********************
     * ANIMATION MONTAGES *
     **********************/
    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* HitReactMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Montages")
    UAnimMontage* DeathMontage;

    /**************************
     * PLAY MONTAGE FUNCTIONS *
     **************************/
    void PlayMontageHitReact(const FName& AttackName);

    /*************
     * SFX / VSF *
     *************/
    UPROPERTY(EditAnywhere, Category = "Sound Effects")
    USoundBase* HitSound;

    UPROPERTY(EditAnywhere, Category = "Visual Effects")
    UParticleSystem* HitParticles;

    /******************
     * DAMAGE / DEATH *
     ******************/
    void DirectionalHitReact(const FVector& ImpactPoint);

    UPROPERTY()
    AActor* CombatTarget;

    UPROPERTY(EditAnywhere)
    double CombatRadius = 500.f;

    UPROPERTY(EditAnywhere)
    double AttackRadius = 150.f;

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
