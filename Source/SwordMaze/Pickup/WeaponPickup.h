// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/Pickup.h"
#include "Weapon/BaseWeapon.h"
#include "WeaponPickup.generated.h"

/**
 * 
 */
UCLASS()
class SWORDMAZE_API AWeaponPickup : public APickup
{
	GENERATED_BODY()
	
public:
	AWeaponPickup();

	virtual void OnPickup_Implementation(APawn* Insigator) override;

public:
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<ABaseWeapon> WeaponClass;

};
