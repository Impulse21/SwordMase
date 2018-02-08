// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponPickup.h"




AWeaponPickup::AWeaponPickup()
{

}

void AWeaponPickup::OnPickup_Implementation(APawn* Insigator)
{
	Super::OnPickup_Implementation(Insigator);

	UE_LOG(LogTemp, Warning, TEXT("Weapon pick up called"));
}