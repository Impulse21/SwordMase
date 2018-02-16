// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseWeapon.h"
#include "Player/HeroCharacter.h"
#include "Components/StaticMeshComponent.h"

ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (!RootComponent)
	{
		Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Mesh"));
		RootComponent = Mesh;
	}

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InventorySlot = EItemType::EI_Primary;
}

void ABaseWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Began Overlap with  %s"), *OtherActor->GetName());
	if (OtherActor)
	{
		DamageActor(OtherActor);
	}
}

void ABaseWeapon::OnEquip_Implementation(AHeroCharacter* NewOwner)
{
	UE_LOG(LogTemp, Warning, TEXT("On Equiped is called"));
	if (NewOwner)
	{
		SetPawnOwner(NewOwner);
		AttachMeshToPawn();
		GetWorld()->DestroyActor<ABaseWeapon>();
	}
}

void ABaseWeapon::OnUnEquip_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("On UnEquiped is called"));

	SetPawnOwner(nullptr);
	DetachMeshFromPawn();

}

AHeroCharacter * ABaseWeapon::GetPawnOwner() const
{
	return MyPawn;
}

void ABaseWeapon::SetPawnOwner(AHeroCharacter * pawn)
{
	if (MyPawn != pawn)
	{
		MyPawn = pawn;
	}
}

void ABaseWeapon::AttachMeshToPawn()
{
	if (MyPawn)
	{
		USkeletalMeshComponent* PawnMesh = MyPawn->GetMesh();
		FName AttachPoint = MyPawn->GetInventoryAttachPoint(InventorySlot);
		Mesh->SetHiddenInGame(false);
		Mesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPoint);
	}
}

void ABaseWeapon::DetachMeshFromPawn()
{
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Mesh->SetActive(true);
}

void ABaseWeapon::DamageActor(AActor * HitActor)
{
	/*
	// Check if this actor is damageable
	if (HitActor->bCanBeDamaged && !HitActor->ActorHasTag("Friendly"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Causing Damage to actor %s"), *HitActor->GetName());
		if (MyPawn->IsAttacking())
		{
			FPointDamageEvent dmgEvent;
			dmgEvent.DamageTypeClass = swordDamageType;
			dmgEvent.Damage = SwordDamage;
			HitActor->TakeDamage(dmgEvent.Damage, dmgEvent, MyPawn->GetController(), this);
		}
	}
	*/
}