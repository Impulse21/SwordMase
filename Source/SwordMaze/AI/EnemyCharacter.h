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

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintPure, Category = EnemyAI)
	FORCEINLINE class UBehaviorTree* GetBotBehavior() { return BotBehavior; };

protected:
	UFUNCTION(BlueprintNativeEvent, Category = EnemyAI)
	void OnSeePlayer(APawn* Pawn);

	/** Components			*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	class UPawnSensingComponent* PawnSensingComp;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Enemy)
	class UBehaviorTree* BotBehavior;

};
