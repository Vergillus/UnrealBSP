// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnActor.h"

// Sets default values
ASpawnActor::ASpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;

	/*static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeObj(TEXT("StaticMesh'/Game/Cube.Cube'"));
	if (CubeObj.Succeeded())
	{
	  MeshComponent->SetStaticMesh(CubeObj.Object);  
	}

	MeshComponent->SetCollisionProfileName(FName("No Collision"),false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetGenerateOverlapEvents(false);*/
}

void ASpawnActor::CreateDynamicMaterial(UMaterialInterface* Mat)
{
	if(!Mat) return;
	
	DynamicMat = UMaterialInstanceDynamic::Create(Mat,this);

	MeshComponent->SetMaterial(0,DynamicMat);
	
}

