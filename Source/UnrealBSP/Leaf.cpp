#include "Leaf.h"

#include "DrawDebugHelpers.h"
#include "DungeonCreator.h"
#include "SpawnActor.h"

Leaf::Leaf(const int32 X, const int32 Y, const int32 Width, const int32 Depth, const int32 Scale, UWorld* CurrentWorld, ASpawnActor* SpawnActor, UMaterialInterface* Material)
{
	XPos = X;
	YPos = Y;
	this->Width = Width;
	this->Depth = Depth;
	this->Scale = Scale;

	RoomMin = 5;
	
	World = CurrentWorld;
	ActorToSpawn = SpawnActor;
	Mat = Material;

	Ratio = 1.2f;
}

bool Leaf::Split()
{
	if (Width <= RoomMin && Depth <= RoomMin) return false;

	bool bCanSplitHorizontal = FMath::RandBool();

	if(Width > Depth && Width / Depth >= Ratio) bCanSplitHorizontal = false;
	else if(Depth > Width && Depth / Width >= Ratio) bCanSplitHorizontal = true;

	const int32 RoomMax = (bCanSplitHorizontal ? Depth : Width) - RoomMin;
	if (RoomMax <= RoomMin) return false;	
	
	if (bCanSplitHorizontal)	// Split Horizontal (from Y axis)
	{
		const int32 RndDepth = FMath::RandRange(RoomMin,RoomMax);
		LeftChild = MakeShared<Leaf>(XPos,YPos,Width, RndDepth,Scale,World,ActorToSpawn,Mat);
		RightChild = MakeShared<Leaf>(XPos, YPos + RndDepth,Width,Depth - RndDepth,Scale,World,ActorToSpawn,Mat);	
	}
	else	// Split Vertical (from X Axis)
	{
		const int32 RndWidth = FMath::RandRange(RoomMin,RoomMax);
		LeftChild = MakeShared<Leaf>(XPos,YPos,RndWidth, Depth,Scale,World,ActorToSpawn,Mat);
		RightChild = MakeShared<Leaf>(XPos  + RndWidth, YPos,Width - RndWidth, Depth,Scale,World,ActorToSpawn,Mat);
	}

	return true;
}


void Leaf::Draw(TArray<F2DArray>& Map,const bool bCanDrawSegments) const 
{
	if (bCanDrawSegments)
	{
		const FLinearColor Color = FLinearColor::MakeRandomColor();
	
		for (int x = XPos; x < Width + XPos; ++x)
		{
			for (int y = YPos; y < Depth + YPos; ++y)
			{
				FTransform Transform;
				Transform.SetLocation(FVector(x * Scale * UEDefaultUnit, y * Scale * UEDefaultUnit,0));
				Transform.SetRotation(FQuat::Identity);
				Transform.SetScale3D(FVector(Scale,Scale,Scale));		

				FActorSpawnParameters SpawnParams;

				//DrawDebugSolidBox(World,Transform.GetLocation(),Transform.GetScale3D(),FColor::Blue,true);
				ASpawnActor* CubeClone = World->SpawnActor<ASpawnActor>(ActorToSpawn->StaticClass(),Transform,SpawnParams);
				CubeClone->CreateDynamicMaterial(Mat);
				CubeClone->GetMaterial()->SetVectorParameterValue("Color",Color);
			}
		}	
	}
	else
	{

		const int32 WallSize = FMath::RandRange(1,3);
		// Create rooms by assigning Map's array values to 0 via using a margin.
		// This we can create room from middle and leaving the edges of the every leaf.
		for (int x = XPos + WallSize; x < Width + XPos - WallSize; ++x)
		{
			for (int y = YPos + WallSize; y < Depth + YPos - WallSize; ++y)
			{
				Map[x].Arr[y] = 0;		
			}
		}		
	}
	
}


Leaf::~Leaf()
{
	
}
