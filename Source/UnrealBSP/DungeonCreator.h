// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "DungeonCreator.generated.h"

USTRUCT()
struct F2DArray
{
	GENERATED_BODY()
public:
	TArray<uint8> Arr;

	F2DArray(){};
	
	F2DArray(unsigned int Size, unsigned int Val)
	{
		Arr.Init(Val,Size);
	}

	uint8 operator[](const uint8 i)
	{
		return Arr[i];
	}	

	void Add(const uint8 Val)
	{
		Arr.Add(Val);
	}
};

UCLASS()
class UNREALBSP_API ADungeonCreator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonCreator();

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Map Settings")
	int32 MapWidth;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Map Settings")
	int32 MapDepth;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Map Settings")
	float Scale;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Map Settings")
	int RecursionDepth;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category= "Map Settings")
	//class ASpawnActor* CubeToSpawn;
	class ADungeonWall* CubeToSpawn;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category= "Map Settings")
	class ADungeonFloor* PlaneToSpawn;

	// Default Cube material
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Map Settings")
	UMaterialInterface* WallMaterial;

	// Default Plane material
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Map Settings")
	UMaterialInterface* FloorMaterial;

	// Subdivides a Leaf node recursively.
	void BSP(TSharedPtr<class Leaf> L, int SplitDepth);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	TArray<F2DArray> Map;

	void DrawMap(const bool bDebug = false);
	
	void Line(int32 X1, int32 Y1, const int32 X2, const int32 Y2,const bool bDebug = false);

	void AddCorridors();

	void AddRandomCorridors(const int32 NumHalls);

	// Optimise the dungeon creation via using ISMs
	void Finalize();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UInstancedStaticMeshComponent* ISMWalls;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UInstancedStaticMeshComponent* ISMFloors;

	TArray<FTransform> Walls;
	
	TArray<FTransform> Floor;

	UPROPERTY(BlueprintReadOnly)
	USceneComponent* EmptyRoot;

	// Should draw the debug data or not
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Map Settings")
	bool bShouldDebug;

	// how many random corridors should be added
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Map Settings")
	int32 RndCorridorsCnt;

	// Default unit size of UE 100 unit(cm) = 1 metre
	const float UEDefaultUnit = 100.0f;

private:
	TArray<FVector2D> Corridors;
};
