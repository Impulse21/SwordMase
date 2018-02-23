// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeGenerator.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Engine/World.h"
#include "Pickup/Pickup.h"

// Sets default values
AMazeGenerator::AMazeGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Subobject"));
	}

	FloorTile = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor Mesh"));
	FloorTile->AttachTo(RootComponent);

	WallTile = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Wall Mesh"));
	WallTile->AttachTo(RootComponent);

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

	FString FileContents;
	if (!GetLevelDataFromFile(FileContents))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get file data"));
		return;
	}

	BuildMapMemory(FileContents);

	DrawMap();
}


// Called when the game starts or when spawned
void AMazeGenerator::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Number of Spawn Locations %d"), SpawnLocation.Num());

	if (Pickup)
	{
		for (FVector pos : SpawnLocation)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawning Actor"));
			SpawnPickup(pos);
		}
	}
}


void AMazeGenerator::LoadLevelData()
{
	FString FileConent;
	if (!GetLevelDataFromFile(FileConent))
	{
		return;
	}
}

bool AMazeGenerator::GetLevelDataFromFile(FString&  FileContent)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (!FFileHelper::LoadFileToString(FileContent, *LevelDataFile))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load tile %s"), *LevelDataFile);
		return false;
	}

	return true;
}

void AMazeGenerator::BuildMapMemory(FString & MapData)
{
	// Clear Current Map Data
	Map.Empty();

	FTileMapRow Row;
	for (int i = 0; i < MapData.Len(); i++)
	{
		FString currChar = MapData.Mid(i, 1);
		if (currChar.Equals("\r"))
		{
			continue;
		}

		if (currChar.Equals("\n"))
		{
			Map.Push(Row);
			Row.Data.Empty();
			continue;
		}

		int tileValue = FCString::Atoi(*currChar);

		Row.Data.Push(static_cast<ETileType>(tileValue));
	}

	// Push last row
	Map.Push(Row);
}

void AMazeGenerator::DrawMap()
{
	const int MaxRows = Map.Num();
	int X = 0;
	int Y = 0;

	//Clear current map
	CleanMap();

	for (int i = 0; i < MaxRows; i++)
	{
		for (auto& tileType : Map[i].Data)
		{
			const FIntVector Position(X, Y, 0);
			const FTransform InstanceTranfrom(static_cast<FVector>(Position));

			switch (tileType)
			{
			case ETileType::TT_Wall:
				WallTile->AddInstance(InstanceTranfrom);
				FloorTile->AddInstance(InstanceTranfrom);
				break;

			case ETileType::TT_Floor:
				FloorTile->AddInstance(InstanceTranfrom);
				SpawnLocation.Push(InstanceTranfrom.GetLocation() + GetActorLocation());
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("Unkown tile %d"), static_cast<int>(tileType));
				break;
			}

			X += TileSize;
		}

		X = 0;
		Y += TileSize;
	}
}

void AMazeGenerator::SpawnPickup(FVector const& Position)
{
	if (Pickup)
	{
		APickup* NewInstance = GetWorld()->SpawnActor<APickup>(Pickup, Position + GetActorLocation(), FRotator::ZeroRotator);
	}
}

void AMazeGenerator::CleanMap()
{
	WallTile->ClearInstances();
	FloorTile->ClearInstances();
	SpawnLocation.Empty();
}