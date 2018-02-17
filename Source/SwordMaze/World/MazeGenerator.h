// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeGenerator.generated.h"

UENUM()
enum class ETileType : uint8
{
	TT_Floor,
	TT_Wall
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

protected: // Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Maze)
	class UInstancedStaticMeshComponent* FloorTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Maze)
	int MaxX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Maze)
	int MaxY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Maze)
	int TileSize;

private:
	UPROPERTY(Transient)
	TArray<ETileType> Map;
};
