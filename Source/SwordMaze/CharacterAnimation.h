// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAnimation.generated.h"

USTRUCT(Blueprintable)
struct FCharacterAnimationInfo
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Blocking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool TryingToAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool TryingToSprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool UsingMagic;
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterAnimation : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SWORDMAZE_API ICharacterAnimation
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Player)
	void GetCharacterInfo(FCharacterAnimationInfo& animInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Player)
	void AttackStartEnd(bool Attacking);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Player)
	void EndAnimInfo(bool FreeToAnimate, bool LockRotation);
};

