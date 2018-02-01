// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WalkSpeed = 600.0f;
	RunSpeed = 1000.0f;

	Health = 100.0f;

	bIsDead = false;

	// Temp
	bCanBeDamaged = true;
}

void ABaseCharacter::SetSprinting(bool sprinting)
{
	Sprinting = sprinting;
}

bool ABaseCharacter::IsSprinting()
{
	return Sprinting && !GetVelocity().IsZero();
}

void ABaseCharacter::ToggleDefend(bool defend)
{
	Blocking = defend;
}

float ABaseCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (bIsDead)
	{
		return 0.0f;
	}

	const float actualDmg = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (actualDmg > 0.0f)
	{
		CalculateHealth(actualDmg);

		if (bIsDead)
		{
			OnDeath(DamageCauser, DamageEvent);
		}
	}

	return actualDmg;
}

void ABaseCharacter::OnDeath(AActor* DamageCauser, FDamageEvent const& DamageEvent)
{

	DetachFromControllerPendingDestroy();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	if (DeathAnimation)
	{
		PlayAnimMontage(DeathAnimation);
	}

	SetLifeSpan(4.0f);
}

void ABaseCharacter::CalculateHealth(float delta)
{
	Health -= delta;
	UE_LOG(LogTemp, Warning, TEXT("I %s health is affected: %f"), *this->GetName(), Health);
	CalculateDead();
}

void ABaseCharacter::CalculateDead()
{
	bIsDead = (Health <= 0);
}
