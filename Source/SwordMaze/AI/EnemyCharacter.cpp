// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "Player/HeroCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

/* AI Include */
#include "Perception/PawnSensingComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	MeleeCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Melee Collision Comp"));
	MeleeCollisionComp->AttachTo(RootComponent);

	SensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Comp"));
	SensingComp->SetPeripheralVisionAngle(40.0f);
	SensingComp->SightRadius = 1500;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	/* This is the earliest moment we can bind our delegates to the component */
	if (SensingComp)
	{
		SensingComp->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSeePlayer);
	}

	if (MeleeCollisionComp)
	{
		MeleeCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnMeleeCompBeginOverlap);
	}
}

void AEnemyCharacter::OnSeePlayer(APawn* Pawn)
{
	LastSeenTime = GetWorld()->GetTimeSeconds();
	SensedPawn = true;

	AEnemyAIController* AiController = Cast<AEnemyAIController>(GetController());
	AHeroCharacter* SensedPawn = Cast<AHeroCharacter>(Pawn);

	if (AiController && SensedPawn && !SensedPawn->IsDead())
	{

		AiController->SetTargetEnemy(SensedPawn);
	}
}

void AEnemyCharacter::PerformMeleeStrike(AActor* HitActor)
{
	if (HitActor && HitActor != this && !IsDead())
	{
		SimulateMeleeStrike();
	}
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (SensedPawn && (GetWorld()->TimeSeconds - LastSeenTime) > SensedTimeout)
	{
		AEnemyAIController* AiController = Cast<AEnemyAIController>(GetController());

		SensedPawn = false;

		if (AiController)
		{
			AiController->SetTargetEnemy(nullptr);
		}
	}

	if (Attacking)
	{
		FName SocketStart;
		FName SocketEnd;

		if (IsAttackingRight)
		{
			SocketStart = TEXT("Right_Trace_Start");
			SocketEnd = TEXT("Right_Trace_End");
		}

		if (IsAttackingLeft)
		{
			SocketStart = TEXT("Left_Trace_Start");
			SocketEnd = TEXT("Left_Trace_End");
		}

		AttackTrace(SocketStart, SocketEnd);
	}
}

void AEnemyCharacter::SimulateMeleeStrike()
{
	PlayAttackAnim();
}

void AEnemyCharacter::OnMeleeCompBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	PerformMeleeStrike(OtherActor);

	GetWorldTimerManager().SetTimer(TimerHandle_MeleeAttack, this, &AEnemyCharacter::OnRetriggerMeleeStrike, MeleeStrikeCooldown, true);
}

void AEnemyCharacter::OnRetriggerMeleeStrike()
{
	TArray<AActor*> Overlaps;
	MeleeCollisionComp->GetOverlappingActors(Overlaps, AHeroCharacter::StaticClass());

	// There should only ever be one Hero. If we don't get a hero End the Timer
	if (Overlaps.Num() != 1)
	{
		TimerHandle_MeleeAttack.Invalidate();
		return;
	}

	PerformMeleeStrike(Overlaps[0]);
}


void AEnemyCharacter::AttackLeftStart_Implementation(bool LeftAttack)
{
	IsAttackingLeft = LeftAttack;
}

void AEnemyCharacter::AttackRightStart_Implementation(bool RightAttack)
{
	IsAttackingRight = RightAttack;
}

void AEnemyCharacter::AttackTrace(FName const& TraceStartSocketName, FName const& TraceEndSocketName)
{
	FVector TraceStart = GetMesh()->GetSocketLocation(TraceStartSocketName);
	FVector TraceEnd = GetMesh()->GetSocketLocation(TraceEndSocketName);

	FCollisionQueryParams TraceWeaponParams = FCollisionQueryParams(FName(TEXT("TraceWeaponParams")), true, this);
	TraceWeaponParams.bTraceComplex = true;
	TraceWeaponParams.bTraceAsyncScene = true;
	TraceWeaponParams.bReturnPhysicalMaterial = false;
	/** End Debug  */

	TArray<FHitResult> HitResults;
	GetWorld()->LineTraceMultiByChannel(HitResults, TraceStart, TraceEnd, ECC_GameTraceChannel14, TraceWeaponParams);

	for (auto& HitResult : HitResults)
	{
		if (HitResult.Actor->IsA<AHeroCharacter>())
		{
			UGameplayStatics::ApplyDamage(HitResult.Actor.Get(), MeleeDamage, this->GetController(), this, MeleeDamageType);
		}
	}
}