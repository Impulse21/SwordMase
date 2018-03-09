// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_GetTargetLocation.h"
#include "AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Player/HeroCharacter.h"
#include "AI/Navigation/NavigationSystem.h"
#include "AI/BotWaypoint.h"
/* This contains includes all key types like UBlackboardKeyType_Vector used below. */
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_GetTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController *Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (Controller == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ABotWaypoint* CurrWaypoint = Controller->GetWaypoint();

	if (CurrWaypoint == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	const float SearchRadius = 10.0f;
	const FVector SearchOrigin = CurrWaypoint->GetActorLocation();
	const FVector Loc = UNavigationSystem::GetRandomReachablePointInRadius(Controller, SearchOrigin, SearchRadius);

	FVector CurrTargetLocation;
	if (Controller->GetTargetLocation(CurrTargetLocation))
	{
		if (Loc != CurrTargetLocation)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), Loc);
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}