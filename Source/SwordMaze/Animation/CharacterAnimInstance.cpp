// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterAnimInstance.h"


UCharacterAnimInstance::UCharacterAnimInstance()
{
	m_currAttackAnim = -1;
}

void UCharacterAnimInstance::Attack_Implementation()
{
	if (AttackAnimations.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Attack Animations initialized"))
			return;
	}

	m_currAttackAnim = (m_currAttackAnim >= AttackAnimations.Num() - 1) ? 0 : m_currAttackAnim + 1;


	UAnimMontage* nextAttackAnim = AttackAnimations[m_currAttackAnim];

	if (nextAttackAnim == nullptr)
	{
		return;
	}

	UAnimMontage* playingAnim = GetCurrentActiveMontage();

	if (playingAnim != nextAttackAnim)
	{
		UE_LOG(LogTemp, Warning, TEXT("Playing Attack Animation[%s]"), *nextAttackAnim->GetName())
			Montage_Play(nextAttackAnim);

	}
}

bool UCharacterAnimInstance::IsPlayingAttackAnim()
{
	if (AttackAnimations.Num() == 0 || m_currAttackAnim < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Attack Animations initialized"));
		return false;
	}

	UAnimMontage* currAnimation = AttackAnimations[m_currAttackAnim];

	return (currAnimation) ? !Montage_GetIsStopped(currAnimation) : false;
}


