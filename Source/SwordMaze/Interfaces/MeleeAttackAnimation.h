// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MeleeAttackAnimation.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMeleeAttackAnimation : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SWORDMAZE_API IMeleeAttackAnimation
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Player)
	void AttackLeftStart(bool LeftAttack);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Player)
	void AttackRightStart(bool RightAtttack);
};
