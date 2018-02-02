// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class SWORDMAZE_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	/** Event that is used for pickups										*/
	UFUNCTION(BlueprintNativeEvent, Category = Pickup)
	void OnPickup(APawn* Insigator);

protected:
	/** Disables mesh from the world										*/
	UFUNCTION(BlueprintCallable, Category = Pickup)
	void HideMesh();

	/** Plays pickup sounds													*/
	UFUNCTION(BlueprintCallable, Category = Pickup)
	void PlayPickupSound();

protected:
	/** Static Mesh for the Pickup											*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	class UStaticMeshComponent* BodyMesh;

	/** Sound that is made when the Item is picked up						*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup")
	class USoundCue* PickupSound;
};
