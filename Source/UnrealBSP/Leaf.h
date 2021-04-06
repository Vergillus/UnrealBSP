#pragma once

class Leaf
{
public:

	int32 XPos;
	int32 YPos;
	int32 Width;
	int32 Depth;
	int32 Scale;
	int32 RoomMin;	//Smallest Room size 

	float Ratio;	// The const aspect ratio between width and depth
	
	UWorld* World;
	class ASpawnActor* ActorToSpawn;
	UMaterialInterface* Mat;

	TSharedPtr<Leaf> LeftChild;
	TSharedPtr<Leaf> RightChild;

	//constructor
	explicit Leaf(const int32 X, const  int32 Y, const  int32 Width, const  int32 Depth, const int32 Scale, UWorld* CurrentWorld, ASpawnActor* SpawnActor, UMaterialInterface* Material);
	~Leaf();

	bool Split();
	
	void Draw(TArray<struct F2DArray>& Map, const bool bCanDrawSegments = false) const;

private:

	// Default unit size of UE 100 unit(cm) = 1 metre
	const float UEDefaultUnit = 100.0f;
	
};
