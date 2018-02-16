// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEquipable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SWORDMAZE_API IEquipable
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnEquip(class AHeroCharacter* NewOwner) PURE_VIRTUAL(IEquipable::OnEquip, );
	virtual void OnUnEquip() PURE_VIRTUAL(IEquipable::OnUnEquip, );
};
