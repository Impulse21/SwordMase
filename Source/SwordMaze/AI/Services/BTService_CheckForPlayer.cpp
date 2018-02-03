// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckForPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/EnemyAIController.h"
#include "Player/HeroCharacter.h"




UBTService_CheckForPlayer::UBTService_CheckForPlayer()
{
	bCreateNodeInstance = true;
}

void UBTService_CheckForPlayer::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	AEnemyAIController* EnemyPC = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (EnemyPC == nullptr)
	{
		return;
	}

	AHeroCharacter* Hero = Cast<AHeroCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Hero)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BlackboardKey.SelectedKeyName, Hero);
	}
}