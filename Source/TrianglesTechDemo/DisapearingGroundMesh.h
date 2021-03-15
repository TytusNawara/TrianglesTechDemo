// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DisapearingGroundMesh.generated.h"

UCLASS()
class ADisapearingGroundMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADisapearingGroundMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void ExpandMesh();

	UMaterial* materialFromStaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* CustomMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* MaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* meshToCopy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float disappearSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D positionOfPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float renderCircleRadius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float triangleScalingSpeed = 1.0f;


	/* The vertices of the mesh */
	TArray<FVector> Vertices;
	TArray<FVector> VerteciesAtBegining;
	
	TArray<float> ScaleOfTriangles;

	/* The triangles of the mesh */
	TArray<int32> Triangles;

	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FColor> vertexColors;
	TArray<FProcMeshTangent> tangents;

	/* Creates a triangle that connects the given vertices */
	void AddTriangle(int32 V1, int32 V2, int32 V3);

	void LoadDataFromStaticMesh();
	void PrepareProceduralMeshForSplit();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
