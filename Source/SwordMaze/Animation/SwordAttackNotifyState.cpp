// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordAttackNotifyState.h"
#include "Player/HeroCharacter.h"

void USwordAttackNotifyState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	UE_LOG(LogTemp, Warning, TEXT("Sword Notify Begin"));
}

void USwordAttackNotifyState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	AHeroCharacter* Hero = Cast<AHeroCharacter>(MeshComp);

	if (Hero)
	{
		Hero->TraceWeapon();
	}
}

void USwordAttackNotifyState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	UE_LOG(LogTemp, Warning, TEXT("Sword Notify End"));
}