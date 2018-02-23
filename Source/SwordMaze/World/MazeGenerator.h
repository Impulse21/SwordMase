// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeGenerator.generated.h"

UENUM()
enum class ETileType : uint8
{
	TT_Floor,
	TT_Wall,
	TT_FloorWithPickup
};

USTRUCT()
struct FTileMapRow
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<ETileType> Data;

};

UCLASS()
class SWORDMAZE_API AMazeGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazeGenerator();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void LoadLevelData();

	bool GetLevelDataFromFile(FString& FileContent);

	void BuildMapMemory(FString& MapData);

	void DrawMap();

	UFUNCTION(BlueprintCallable, Category = Maze)
	void SpawnPickup(FVector const& Position);

	UFUNCTION(BlueprintCallable, Category = Maze)
	void CleanMap();

protected: // Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Maze)
	class UInstancedStaticMeshComponent* FloorTile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Maze)
	class UInstancedStaticMeshComponent* WallTile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Maze)
	TSubclassOf<class APickup> Pickup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Maze)
	FString LevelDataFile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Maze)
	int TileSize;

	UPROPERTY(Transient)
	TArray<FTileMapRow> Map;

	UPROPERTY(Transient)
	TArray<class APickup*> PickupInstances;
};
