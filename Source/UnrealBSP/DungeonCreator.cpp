// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonCreator.h"
#include "Leaf.h"
//#include "SpawnActor.h"
#include "DungeonWall.h"
#include "DungeonFloor.h"

// Sets default values
ADungeonCreator::ADungeonCreator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	EmptyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = EmptyRoot;
	
	
	ISMWalls = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Dungeon Walls"));
	ISMFloors = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Dungeon Floor"));

	ISMWalls->SetupAttachment(RootComponent);
	ISMFloors->SetupAttachment(RootComponent);

	MapWidth = 50;
	MapDepth = 50;

	Scale = 2;
	RecursionDepth = 3;

	bShouldDebug = false;
	RndCorridorsCnt = 10;
}

// Called when the game starts or when spawned
void ADungeonCreator::BeginPlay()
{
	Super::BeginPlay();	

	// Allocate memory for 2D Array
	Map.SetNum(MapWidth);	
	for (int i = 0; i < MapWidth; ++i)
	{
		Map[i].Arr.SetNum(MapDepth);		
	}	

	// Set all array elements to 1
	for (int y = 0; y < MapDepth; ++y)
	{
		for (int x = 0; x < MapWidth; ++x)
		{
			Map[x].Arr[y] = 1;
		}
	}

	CubeToSpawn = NewObject<ADungeonWall>(ADungeonWall::StaticClass());
	PlaneToSpawn = NewObject<ADungeonFloor>(ADungeonFloor::StaticClass());

	const TSharedPtr<Leaf> Root(new Leaf(0,0,MapWidth,MapDepth,Scale,GetWorld(),CubeToSpawn,WallMaterial));
	BSP(Root,RecursionDepth);
	
	AddCorridors();
	AddRandomCorridors(RndCorridorsCnt);
	
	DrawMap(bShouldDebug);
	
	Finalize();
}

void ADungeonCreator::BSP(TSharedPtr<class Leaf> L, int SplitDepth)
{
	if(L == nullptr) return;
	if(SplitDepth <= 0)
	{
		if (L)
		{
			L->Draw(Map,bShouldDebug);
			//Get the center of the leaf and populate the corridors array whit this position
			const FVector2D CenterRoomPos = FVector2D(L->XPos + L->Width / 2.0f, L->YPos + L->Depth / 2.0f);
			Corridors.Add(CenterRoomPos);
		}
		
		return;
	}

	if (L->Split())
	{
		BSP(L->LeftChild,SplitDepth - 1);
		BSP(L->RightChild, SplitDepth - 1);
	}
	else // Draw only the final level of leaf nodes
	{
		if (L)
		{
			L->Draw(Map,bShouldDebug);
			//Get the center of the leaf and populate the corridors array whit this position
			const FVector2D CenterRoomPos = FVector2D(L->XPos + L->Width / 2.0f, L->YPos + L->Depth / 2.0f);
			Corridors.Add(CenterRoomPos);
		}
	}	
}

void ADungeonCreator::AddCorridors()
{
	for (int i = 1; i < Corridors.Num(); ++i)
	{
		if(((int)Corridors[i].X == (int)Corridors[i - 1].X) || ((int)Corridors[i].Y == (int)Corridors[i - 1].Y))	// draw/create only the straight lines (corridors)
			Line((int)Corridors[i].X,(int)Corridors[i].Y,(int)Corridors[i - 1].X,(int)Corridors[i - 1].Y);
		else	// draw horizontal lines as straight lines in order to prevent having stranded rooms
		{
			Line((int)Corridors[i].X,(int)Corridors[i].Y,(int)Corridors[i].X,(int)Corridors[i - 1].Y);	// Same Xs different Ys
			Line((int)Corridors[i].X,(int)Corridors[i].Y,(int)Corridors[i - 1].X,(int)Corridors[i].Y);	// Same Ys different Xs
		}
	}
}

void ADungeonCreator::AddRandomCorridors(const int32 NumHalls)
{
	for (int i = 0; i < NumHalls; ++i)
	{
		int32 StartX = FMath::RandRange(5, MapWidth - 5);
		int32 StartY = FMath::RandRange(5, MapDepth - 5);
		int32 EndX = FMath::RandRange(5, MapWidth - 5);
		int32 EndY = FMath::RandRange(5, MapDepth - 5);

		if (FMath::RandRange(0,1) == 0)
		{
			Line(StartX,StartY,EndX,StartY);
		}
		else
		{
			Line(StartX,StartY,StartX,EndY);
		}			
	}
}


void ADungeonCreator::DrawMap(const bool bDebug)
{
	for (int y = 0; y < MapDepth; ++y)
	{
		for (int x = 0; x < MapWidth; ++x)
		{
			if(Map[x][y] == 0)	//Create floor
			{
				FTransform Transform;
				Transform.SetLocation(FVector(x * Scale * UEDefaultUnit, y * Scale * UEDefaultUnit,400));
				Transform.SetRotation(FQuat::Identity);
				Transform.SetScale3D(FVector(Scale,Scale,Scale));

				Floor.Add(Transform);

				
				/*FActorSpawnParameters SpawnParams;
				
				ADungeonFloor* FloorPlane = GetWorld()->SpawnActor<ADungeonFloor>(PlaneToSpawn->StaticClass(),Transform,SpawnParams);
				if (FloorPlane && FloorMaterial)
				{
					FloorPlane->GetMesh()->SetMaterial(0,FloorMaterial);					
				}*/
			}			
			else if (Map[x][y] == 1)	// Create wall
			{
				FTransform Transform;
				Transform.SetLocation(FVector(x * Scale * UEDefaultUnit, y * Scale * UEDefaultUnit,500));
				Transform.SetRotation(FQuat::Identity);
				Transform.SetScale3D(FVector(Scale,Scale,Scale));

				Walls.Add(Transform);
				
				/*FActorSpawnParameters SpawnParams;

				//DrawDebugSolidBox(World,Transform.GetLocation(),Transform.GetScale3D(),FColor::Blue,true);
				ADungeonWall* MapCube = GetWorld()->SpawnActor<ADungeonWall>(CubeToSpawn->StaticClass(),Transform,SpawnParams);
				if (MapCube && WallMaterial)
				{
					MapCube->GetMesh()->SetMaterial(0,WallMaterial);
					
				}*/
				
			}
			else if (Map[x][y] == 2 && bDebug)	//Draw lines/corridors if desired
			{
				FTransform Transform;
				Transform.SetLocation(FVector(x * Scale * UEDefaultUnit, y * Scale * UEDefaultUnit,500));
				Transform.SetRotation(FQuat::Identity);
				Transform.SetScale3D(FVector(Scale,Scale,Scale));		

				FActorSpawnParameters SpawnParams;

				//DrawDebugSolidBox(World,Transform.GetLocation(),Transform.GetScale3D(),FColor::Blue,true);
				ADungeonWall* Line = GetWorld()->SpawnActor<ADungeonWall>(CubeToSpawn->StaticClass(),Transform,SpawnParams);
				Line->CreateDynamicMaterial(WallMaterial);
				Line->GetMaterial()->SetVectorParameterValue("Color",FLinearColor::Red);
			}			
		}
	}

}

// Adapted Bresenhan's line algorithm
// from https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void ADungeonCreator::Line(int32 X1, int32 Y1, const int32 X2, const int32 Y2,const bool bDebug)
{
	int32 W = X2 - X1;
	int32 H = Y2 - Y1;
	int32 DX1 = 0, DY1 = 0, DX2 = 0, DY2 = 0;

	if(W < 0) DX1 = -1; else if(W > 0) DX1 = 1;
	if(H < 0) DY1 = -1; else if(H > 0) DY1 = 1;
	if(W < 0) DX2 = -1; else if(W > 0) DX2 = 1;

	int32 Longest = FMath::Abs(W);
	int32 Shortest = FMath::Abs(H);

	if (Longest < Shortest)
	{
		Longest = FMath::Abs(H);
		Shortest = FMath::Abs(W);

		if(H < 0) DY2 = -1; else if(H > 0) DY2 = 1;
	}

	int32 Numerator = Longest >> 1;

	for (int i = 0; i <= Longest; ++i)
	{
		if(bDebug) 	Map[X1].Arr[Y1]= 2;		// draw corridors if desired
		else		Map[X1].Arr[Y1]	= 0;

		Numerator += Shortest;

		if (Numerator > Longest)
		{
			Numerator -= Longest;
			X1 += DX1;
			Y1 += DY1;
		}
		else
		{
			X1 += DX2;
			Y1 += DY2;
		}
	}
	
}

void ADungeonCreator::Finalize()
{
	// Populate ISMWalls
	if (ISMWalls && Walls.Num() > 0)
	{
		ISMWalls->SetStaticMesh(CubeToSpawn->GetMesh()->GetStaticMesh());
		ISMWalls->SetMaterial(0,WallMaterial);
		for(FTransform Element : Walls)
		{
			ISMWalls->AddInstanceWorldSpace(Element);			
		}
		ISMWalls->RegisterComponent();
	}

	// Populate ISMFloor
	if (ISMFloors && Floor.Num() > 0)
	{
		ISMFloors->SetStaticMesh(PlaneToSpawn->GetMesh()->GetStaticMesh());
		ISMFloors->SetMaterial(0,FloorMaterial);
		for(FTransform Element : Floor)
		{
			ISMFloors->AddInstanceWorldSpace(Element);			
		}
		ISMFloors->RegisterComponent();
	}

	Walls.Empty();
	Floor.Empty();
	Corridors.Empty();	
}



