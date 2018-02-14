// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponPickup.h"
#include "Player/HeroCharacter.h"
#include "Weapon/BaseWeapon.h"



AWeaponPickup::AWeaponPickup()
{

}

void AWeaponPickup::OnPickup_Implementation(APawn* PawnInstigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Weapon pick up called"));

	// Check if we are being picked up by 
	AHeroCharacter* heroPawn = Cast<AHeroCharacter>(PawnInstigator);
	if (heroPawn)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABaseWeapon* NewWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, SpawnInfo);

		//
		//heroPawn->
		Super::OnPickup_Implementation(Instigator);
	}

}