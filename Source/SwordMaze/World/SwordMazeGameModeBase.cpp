// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordMazeGameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Player/HeroCharacter.h"

ASwordMazeGameModeBase::ASwordMazeGameModeBase()
{
	/*
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/RPG/Blueprints/BP_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	*/

	Score = 0;
}

void ASwordMazeGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}

	auto hero = Cast<AHeroCharacter>(DefaultPawnClass);

	if (hero)
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting up callback"));
		hero->UpdatePlayerScore.AddDynamic(this, &ASwordMazeGameModeBase::IncrementScore);
	}
}

void ASwordMazeGameModeBase::IncrementScore(int DeltaScore)
{
	UE_LOG(LogTemp, Warning, TEXT("Update Score Event"));
	Score += DeltaScore;
}
