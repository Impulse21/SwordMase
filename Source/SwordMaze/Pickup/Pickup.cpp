// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "World/SwordMazeGameModeBase.h"


// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	BodyMesh->SetSimulatePhysics(false);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent = BodyMesh;
}

void APickup::OnPickup_Implementation(APawn* Insigator)
{
	PlayPickupSound();
	HideMesh();
	PlayPickupSound();

	Destroy();
}

void APickup::HideMesh()
{
	if (BodyMesh)
	{
		BodyMesh->SetVisibility(false);
		BodyMesh->SetSimulatePhysics(false); // Just in case simulation was turned on, display
		BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void APickup::PlayPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
	}
}
