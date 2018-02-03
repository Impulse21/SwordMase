// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "Player/HeroCharacter.h"

/* AI Include */
#include "Perception/PawnSensingComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Senser"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->bHearNoises = false;
}


void AEnemyCharacter::BeginPlay()
{
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSeePlayer);
	}
}

void AEnemyCharacter::OnSeePlayer_Implementation(APawn* Pawn)
{
	AHeroCharacter* SensedHero = Cast<AHeroCharacter>(Pawn);
	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());

	if(SensedHero && AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("We have sensed the player"));
		AIController->SetTargetEnemy(SensedHero);
	}
}