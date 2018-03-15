// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/BaseCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SWORDMAZE_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

public:
	UFUNCTION(BlueprintPure, Category = EnemyAI)
	FORCEINLINE class UBehaviorTree* GetBotBehavior() { return BotBehavior; };

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	UFUNCTION(BlueprintCallable, Category = Enemy)
	void PerformMeleeStrike(AActor* HitActor);

	UFUNCTION(BlueprintCallable, Category = Enemy)
	void SimulateMeleeStrike();

	UFUNCTION(BlueprintCallable, Category = Enemy)
	void OnMeleeCompBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION(BlueprintCallable, Category = Enemy)
	void OnRetriggerMeleeStrike();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Enemy)
	class UBehaviorTree* BotBehavior;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Enemy)
	class UCapsuleComponent* MeleeCollisionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Enemy)
	class UPawnSensingComponent* SensingComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Enemy)
	float SensedTimeout;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Enemy)
	float MeleeDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Enemy)
	float MeleeStrikeCooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Enemy)
	TSubclassOf<UDamageType> MeleeDamageType;

private:
	UPROPERTY(Transient)
	FTimerHandle TimerHandle_MeleeAttack;

	UPROPERTY(Transient)
	float LastSeenTime;

	UPROPERTY(Transient)
	bool SensedPawn;
};
