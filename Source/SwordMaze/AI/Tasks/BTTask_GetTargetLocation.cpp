// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_GetTargetLocation.h"
#include "AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Player/HeroCharacter.h"

EBTNodeResult::Type UBTTask_GetTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController *CharPC = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	AHeroCharacter* Enemy = Cast<AHeroCharacter>
		(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));

	if (Enemy && CharPC)
	{
		FVector position = Enemy->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("Setting Target Location (%f, %f, %f)"), position.X, position.Y, position.Z);

		CharPC->SetTargetLocation(position);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}