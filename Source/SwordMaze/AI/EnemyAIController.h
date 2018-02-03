// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class SWORDMAZE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();
	
	virtual void Possess(class APawn* InPawn) override;
	virtual void UnPossess() override;

public:
	UFUNCTION(BlueprintCallable, Category = EnemyAI)
	class ABaseCharacter* GetTargetEnemy();

	UFUNCTION(BlueprintCallable, Category = EnemyAI)
	void SetTargetEnemy(APawn* NewTarget);

	// Key IDs
public:
	UPROPERTY(EditDefaultsOnly, Category = "EnemyAI")
	FName TargetEnemyKeyName;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyAI)
	class UBehaviorTreeComponent* BehaviorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyAI)
	class UBlackboardComponent* BlackboardComponent;
	
};
