// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeGenerator.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
AMazeGenerator::AMazeGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FloorTile = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor Mesh"));
	RootComponent = FloorTile;

	MaxX = 1;
	MaxY = 1;
	TileSize = 100;
}

void AMazeGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (FloorTile == nullptr || FloorTile->GetStaticMesh() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to generate floor, Floor Tile is null or floor tile static mesh is null"));
		return;
	}

	const int NumOfTiles = MaxX * MaxY;
	Map.SetNum(NumOfTiles, true);

	for (int i = 0; i < NumOfTiles; i++)
	{
		const FIntVector Position((i / MaxY) * TileSize , (i % MaxY) * TileSize, 0);
		const FTransform InstanceTranfrom(static_cast<FVector>(Position));
		FloorTile->AddInstance(InstanceTranfrom);
	}
}


// Called when the game starts or when spawned
void AMazeGenerator::BeginPlay()
{
	Super::BeginPlay();
}

