// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SwordMazeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SWORDMAZE_API ASwordMazeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASwordMazeGameModeBase();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void IncrementScore();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Score", Meta = (BlueprintProcted = "true"))
	float ScoreToWin;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score", Meta = (BlueprintProcted = "true"))
	float Score;

	/** The widget class to use for the HUD screen				*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Score", Meta = (BlueprintProcted = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	/** The actual instace of the HUD							*/
	UPROPERTY()
	class UUserWidget* CurrentWidget;
};
