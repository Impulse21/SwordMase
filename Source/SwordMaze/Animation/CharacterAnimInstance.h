// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SWORDMAZE_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UCharacterAnimInstance();

	UFUNCTION(BlueprintNativeEvent, Category = CharacterAnim)
		void Attack();


	// Getter and Setters
public:
	UFUNCTION(BlueprintCallable)
		bool IsPlayingAttackAnim();


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CharacterAnim)
		TArray<class UAnimMontage*> AttackAnimations;

protected:
	int m_currAttackAnim;
};
