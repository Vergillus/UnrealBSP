// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonFloor.h"

ADungeonFloor::ADungeonFloor()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneObj(TEXT("StaticMesh'/Game/Plane.Plane'"));
	if (PlaneObj.Succeeded())
	{
		MeshComponent->SetStaticMesh(PlaneObj.Object);  
	}
}
