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

	MaxHealth = 100.0f;

	bIsDead = false;

	// Temp
	bCanBeDamaged = true;
	FreeToAnimate = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
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
	UE_LOG(LogTemp, Warning, TEXT("I have sustained critical damage [%s]"), *this->GetName());
	if (bIsDead)
	{
		return 0.0f;
	}

	const auto ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.0f)
	{
		CalculateHealth(ActualDamage);

		if (bIsDead)
		{
			OnDeath(DamageCauser, DamageEvent);
		}
	}

	return ActualDamage;
}

void ABaseCharacter::OnDeath(AActor* DamageCauser, FDamageEvent const& DamageEvent)
{

	DetachFromControllerPendingDestroy();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	float LifeSpan = 3.0f;
	if (DeathAnimation)
	{
		UE_LOG(LogTemp, Warning, TEXT("Playing Death Animation"));
		LifeSpan += PlayAnimMontage(DeathAnimation);
	}

	UE_LOG(LogTemp, Warning, TEXT("Setting Pawns lifespan to %f"), LifeSpan);
	SetLifeSpan(LifeSpan);
}

void ABaseCharacter::CalculateHealth(float delta)
{
	CurrentHealth -= delta;
	UE_LOG(LogTemp, Warning, TEXT("I %s health is affected: %f"), *this->GetName(), CurrentHealth);
	CalculateDead();
}

void ABaseCharacter::CalculateDead()
{
	bIsDead = (CurrentHealth <= 0);
}

void ABaseCharacter::PlayAttackAnim()
{
	if (!FreeToAnimate)
	{
		UE_LOG(LogTemp, Warning, TEXT("We are not free to animate"));
		return;
	}

	if (AttackAnims.AttackAnimations.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("There are no animations to animate"));
		return;
	}

	uint8 * CurrAnimIndex = &AttackAnims.CurrAnimIndex;
	UAnimMontage* CurrAnim = nullptr;

	CurrAnim = AttackAnims.AttackAnimations[*CurrAnimIndex];

	*CurrAnimIndex += 1;
	if (*CurrAnimIndex >= AttackAnims.AttackAnimations.Num())
	{
		*CurrAnimIndex = 0;
	}


	if (CurrAnim)
	{
		PlayAnimMontage(CurrAnim);
		FreeToAnimate = false;
		FString Name = (CurrAnim) ? *CurrAnim->GetName() : TEXT("");
		UE_LOG(LogTemp, Warning, TEXT("Animation Selected to play is [%s]"), *Name);
	}
}

void ABaseCharacter::GetCharacterInfo_Implementation(FCharacterAnimationInfo & animInfo)
{
}

void ABaseCharacter::AttackStartEnd_Implementation(bool IsAttacking)
{
	Attacking = IsAttacking;
}

void ABaseCharacter::EndAnimInfo_Implementation(bool IsFreeToAnimate, bool LockRotation)
{
	FreeToAnimate = IsFreeToAnimate;
}

