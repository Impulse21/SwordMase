// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "EnemyAIController.generated.h"


/**
*	This is the Bot AI Type
*/
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EBotState : uint8
{
	EI_None		UMETA(DisplayName = "None"),
	EI_Chase 	UMETA(DisplayName = "Chase"),
	EI_Scatter 	UMETA(DisplayName = "Scatter")
};



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

	UFUNCTION(BlueprintCallable, Category = EnemyAI)
	EBotState GetBotState();

	UFUNCTION(BlueprintCallable, Category = EnemyAI)
	void SetNotState(EBotState const& NewState);

	// Key IDs
public:
	UPROPERTY(EditDefaultsOnly, Category = "EnemyAI")
	FName TargetEnemyKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "EnemyAI")
	FName BotStateKeyName;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyAI)
	class UBehaviorTreeComponent* BehaviorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = EnemyAI)
	class UBlackboardComponent* BlackboardComponent;
	
};
