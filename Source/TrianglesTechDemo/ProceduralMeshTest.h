// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshTest.generated.h"

UCLASS()
class AProceduralMeshTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralMeshTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* CustomMesh;

	UPROPERTY(EditAnywhere,  BlueprintReadWrite)
		UMaterial* Material;

	/* The vertices of the mesh */
	TArray<FVector> Vertices;

	/* The triangles of the mesh */
	TArray<int32> Triangles;

	/* Creates a triangle that connects the given vertices */
	void AddTriangle(int32 V1, int32 V2, int32 V3);

	void GenerateCubeMesh();
	void GenerateTerrainMesh();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	int GetIndexFromVerticies(int x, int y);
	int32 spaceBetweenVertecies = 1;
	int32 terrainSize = 100;
	float timePassed = 0;

};
