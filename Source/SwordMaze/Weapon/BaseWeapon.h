// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Equipable.h"
#include "BaseWeapon.generated.h"

/**
*
*/
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EItemType : uint8
{
	EI_Primary 		UMETA(DisplayName = "Weapon"),
	EI_Secondary 	UMETA(DisplayName = "Secondary/Sheild"),
	EI_Armour		UMETA(DisplayName = "Armour"),
	EI_Consumable	UMETA(DisplayName = "Consumable")
};

UCLASS()
class SWORDMAZE_API ABaseWeapon : public AActor, public IEquipable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();
	
	UFUNCTION()
	void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	UFUNCTION(BlueprintNativeEvent, Category = Weapon)
	void OnEquip(class AHeroCharacter* NewOwner) override;

	UFUNCTION(BlueprintPure, Category = Weapon)
	class AHeroCharacter* GetPawnOwner() const;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetPawnOwner(class AHeroCharacter* pawn);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void AttachMeshToPawn();

protected:
	/** Weapon's Mesh			*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

protected:
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void DamageActor(AActor* HitActor);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
	TSubclassOf<UDamageType> swordDamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
	float SwordDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	class AHeroCharacter* MyPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	EItemType InventorySlot;
};
