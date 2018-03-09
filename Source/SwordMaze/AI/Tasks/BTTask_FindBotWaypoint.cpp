// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindBotWaypoint.h"
#include "AI/EnemyAIController.h"
#include "AI/BotWaypoint.h"
#include "Kismet/GameplayStatics.h"
/* This contains includes all key types like UBlackboardKeyType_Vector used below. */
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Engine/EngineTypes.h"


EBTNodeResult::Type UBTTask_FindBotWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* Controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (Controller == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ABotWaypoint* CurrWaypoint = Controller->GetWaypoint();
	AActor* NewWaypoint = nullptr;

	TArray<AActor*> AllWaypoints;
	UGameplayStatics::GetAllActorsOfClass(Controller, ABotWaypoint::StaticClass(), AllWaypoints);

	if (AllWaypoints.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	NewWaypoint = AllWaypoints[FMath::RandRange(0, AllWaypoints.Num() - 1)];

	if (NewWaypoint)
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID(), NewWaypoint);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;

}

void UBTTask_FindBotWaypoint::GetPotentialWaypoints(AActor const* CurrentActor, TArray<AActor*> const& AllActors, TArray<AActor*> PotentalWaypoints)
{

	if (CurrentActor == nullptr || AllActors.Num() == 0)
	{
		return;
	}

	FCollisionQueryParams TraceWallParams = FCollisionQueryParams(FName(TEXT("TraceWeaponParams")), true, CurrentActor);
	TraceWallParams.bTraceComplex = true;
	TraceWallParams.bTraceAsyncScene = true;
	TraceWallParams.bReturnPhysicalMaterial = false;

	const FVector StartLocation = CurrentActor->GetActorLocation();

	for (auto Actor : AllActors)
	{
		const FVector EndLocation = Actor->GetActorLocation();

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_EngineTraceChannel6, TraceWallParams);

		if (HitResult.Actor.Get())
		{
			UE_LOG(LogTemp, Warning, TEXT("We have hit something [%s]"), *HitResult.Actor->GetName());
		}
	}
}