// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "Player/HeroCharacter.h"
#include "Components/CapsuleComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	MeleeCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Melee Collision Comp"));
	MeleeCollisionComp->AttachTo(RootComponent);

}
