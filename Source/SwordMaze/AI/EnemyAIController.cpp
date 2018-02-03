// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyCharacter.h"


AEnemyAIController::AEnemyAIController()
{
	BehaviorComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackbaord"));

	TargetEnemyKeyName = "TargetEnemy";
}

void AEnemyAIController::Possess(APawn * InPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("Controller Possess[%s]"), *InPawn->GetName());

	Super::Possess(InPawn);

	AEnemyCharacter * EnemyChar = Cast<AEnemyCharacter>(InPawn);

	if (EnemyChar && EnemyChar->GetBotBehavior())
	{
		BlackboardComponent->InitializeBlackboard(*EnemyChar->GetBotBehavior()->BlackboardAsset);
		BehaviorComponent->StartTree(*EnemyChar->GetBotBehavior());
	}
}

void AEnemyAIController::UnPossess()
{
	Super::UnPossess();
}

void AEnemyAIController::SetTargetEnemy(APawn* NewTarget)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsObject(TargetEnemyKeyName, NewTarget);
	}
}

ABaseCharacter* AEnemyAIController::GetTargetEnemy()
{
	if (BlackboardComponent == nullptr)
	{
		return nullptr;
	}

	return Cast<ABaseCharacter>(BlackboardComponent->GetValueAsObject(TargetEnemyKeyName));
}

EBotState AEnemyAIController::GetBotState()
{
	if (BlackboardComponent)
	{
		return static_cast<EBotState>(BlackboardComponent->GetValueAsEnum(BotStateKeyName));
	}

	return EBotState::EI_None;
}	

void AEnemyAIController::SetNotState(EBotState const& NewState)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsEnum(BotStateKeyName, static_cast<uint8>(NewState));
	}
}