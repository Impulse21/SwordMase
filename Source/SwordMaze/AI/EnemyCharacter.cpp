// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "Player/HeroCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"

/* AI Include */
#include "Perception/PawnSensingComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	MeleeCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Melee Collision Comp"));
	MeleeCollisionComp->AttachTo(RootComponent);

	SensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Comp"));
	SensingComp->SetPeripheralVisionAngle(40.0f);
	SensingComp->SightRadius = 1500;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	/* This is the earliest moment we can bind our delegates to the component */
	if (SensingComp)
	{
		SensingComp->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSeePlayer);
	}
}

void AEnemyCharacter::OnSeePlayer(APawn* Pawn)
{
	LastSeenTime = GetWorld()->GetTimeSeconds();
	SensedPawn = true;

	AEnemyAIController* AiController = Cast<AEnemyAIController>(GetController());
	AHeroCharacter* SensedPawn = Cast<AHeroCharacter>(Pawn);

	if (AiController && SensedPawn && !SensedPawn->IsDead())
	{

		AiController->SetTargetEnemy(SensedPawn);
	}
}

void AEnemyCharacter::PerformMeleeStrike(AActor* HitActor)
{
	if (LastMeleeStrike > GetWorld()->GetTimeSeconds() - MeleeStrikeCooldown)
	{
		// Cool down timer has not occured
		return;
	}

	if (HitActor && HitActor != this && !IsDead())
	{
		AHeroCharacter* HeroActor = Cast<AHeroCharacter>(HitActor);

		if (HeroActor == nullptr)
		{
			return;
		}

		UGameplayStatics::ApplyDamage(HeroActor, MeleeDamage, this->GetController(), this, MeleeDamageType);
	}
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (SensedPawn && (GetWorld()->TimeSeconds - LastSeenTime) > SensedTimeout)
	{
		AEnemyAIController* AiController = Cast<AEnemyAIController>(GetController());

		SensedPawn = false;

		if (AiController)
		{
			AiController->SetTargetEnemy(nullptr);
		}
	}
}