// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnActor.generated.h"

UCLASS(Abstract)
class UNREALBSP_API ASpawnActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnActor();
	
protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(BlueprintReadOnly)
	UMaterialInstanceDynamic* DynamicMat;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	FORCEINLINE UStaticMeshComponent* GetMesh() const {return MeshComponent;}
	FORCEINLINE UMaterialInstanceDynamic* GetMaterial() const {return DynamicMat;}

	void CreateDynamicMaterial(UMaterialInterface* Mat);	

};
