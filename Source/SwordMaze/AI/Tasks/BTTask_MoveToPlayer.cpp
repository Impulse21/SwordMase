// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveToPlayer.h"
#include "AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Player/HeroCharacter.h"

EBTNodeResult::Type UBTTask_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController *CharPC = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	AHeroCharacter* Enemy = Cast<AHeroCharacter>
		(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));

	if (Enemy && CharPC)
	{
		FVector position = Enemy->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("We are moving to player (%f, %f, %f)"), position.X, position.Y, position.Z);

		if (CharPC->GetOwner())
		{
			FVector charLocation = CharPC->GetOwner()->GetActorLocation();
			UE_LOG(LogTemp, Warning, TEXT("We are moving to player (%f, %f, %f)"), charLocation.X, charLocation.Y, charLocation.Z);

			float distance = FVector::Distance(charLocation, position);
			UE_LOG(LogTemp, Warning, TEXT("Distance is %f"), distance);
		}

		CharPC->MoveToActor(Enemy, 5.0f, true, true, true, 0, true);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

