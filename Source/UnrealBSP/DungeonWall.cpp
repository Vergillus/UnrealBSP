// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonWall.h"

ADungeonWall::ADungeonWall()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeObj(TEXT("StaticMesh'/Game/Cube.Cube'"));
	if (CubeObj.Succeeded())
	{
		MeshComponent->SetStaticMesh(CubeObj.Object);  
	}
	
	MeshComponent->SetCollisionProfileName(FName("No Collision"),false);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComponent->SetGenerateOverlapEvents(false);
}
