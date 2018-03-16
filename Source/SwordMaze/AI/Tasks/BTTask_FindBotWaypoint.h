// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindBotWaypoint.generated.h"

/**
 * 
 */
UCLASS()
class SWORDMAZE_API UBTTask_FindBotWaypoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void GetPotentialWaypoints(AActor const* CurrentActor, TArray<AActor*> const& AllActors, TArray<AActor*> PotentalWaypoints);
};