// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMeshTest.h"

// Sets default values
AProceduralMeshTest::AProceduralMeshTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	SetRootComponent(CustomMesh);
	CustomMesh->bUseAsyncCooking = true;

}

// Called when the game starts or when spawned
void AProceduralMeshTest::BeginPlay()
{
	Super::BeginPlay();
	GenerateTerrainMesh();

}

// Called every frame
void AProceduralMeshTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timePassed += DeltaTime;
	float scale = 0.4f;
	TArray<FColor> VertexColors;


	for (int y = 0; y < terrainSize; y++)
	{
		for (int x = 0; x < terrainSize; x++)
		{
			FVector old = Vertices[GetIndexFromVerticies(x, y)];

			float scareOfNoise = 40.0f;
			// int32 height = 50 * FMath::PerlinNoise2D(FVector2D(x*scareOfNoise, y*scareOfNoise));
			float noise = FMath::PerlinNoise3D(FVector(x / scareOfNoise, y / scareOfNoise, timePassed * scale));
			float height = noise * 600.0f;
			old.Z = height;
			// UE_LOG(LogTemp, Warning, TEXT("Noise %f height %f"), noise, height);
				Vertices[GetIndexFromVerticies(x, y)]  = old;
				//asd
			
				float normalizedNoise = (noise + 1.0) / 2.0;
				// float color = normalizedNoise > 0.5f ? 1.0f : 0.0f;
				float color = FMath::Pow(normalizedNoise, 3);
				// VertexColors.Add(FColor(color, color, color));
				FLinearColor fLinearColor(color, color, color);
				VertexColors.Add(fLinearColor.ToFColor(true));
		}
	}
	

	CustomMesh->UpdateMeshSection(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>());
}

int AProceduralMeshTest::GetIndexFromVerticies(int x, int y)
{
	int index = y * terrainSize;
	index += x;
	return index;
}

void AProceduralMeshTest::AddTriangle(int32 V1, int32 V2, int32 V3)
{
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}


void AProceduralMeshTest::GenerateTerrainMesh()
{
	//6 sides on cube, 4 verts each (corners)

	//These are relative locations to the placed Actor in the world

	TArray<FLinearColor> VertexColors;

	int32 offsetOfTerrain = (terrainSize * spaceBetweenVertecies) / 2;

	for(int y = 0; y < terrainSize; y++)
	{
		for(int x = 0; x < terrainSize; x++)
		{
			float scareOfNoise = 40.0f;
			// int32 height = 50 * FMath::PerlinNoise2D(FVector2D(x*scareOfNoise, y*scareOfNoise));
			float noise = FMath::PerlinNoise2D(FVector2D(x / scareOfNoise, y / scareOfNoise));
			float height = noise * 600.0f;
			// UE_LOG(LogTemp, Warning, TEXT("Noise %f height %f"), noise, height);
			Vertices.Add(FVector((x)*terrainSize - offsetOfTerrain, (y)*terrainSize - offsetOfTerrain, height));
			float normalizedNoise = (noise + 1.0) / 2.0;
			// float color = normalizedNoise > 0.5f ? 1.0f : 0.0f;
			float color = normalizedNoise - 0.2f;
			VertexColors.Add(FLinearColor(color, color, color));
		}
	}

	
	//firts small square
	// AddTriangle(GetIndexFromVerticies(0, 0), GetIndexFromVerticies(0, 1), GetIndexFromVerticies(1, 0));
	// AddTriangle(GetIndexFromVerticies(0, 1), GetIndexFromVerticies(1,1), GetIndexFromVerticies(1, 0));

	for (int y = 0; y < terrainSize -1; y++)
	{
		for (int x = 0; x < terrainSize -1; x++)
		{
			AddTriangle(GetIndexFromVerticies(x, y), GetIndexFromVerticies(x, y +1), GetIndexFromVerticies(x +1, y));
			AddTriangle(GetIndexFromVerticies(x, y +1), GetIndexFromVerticies(x + 1, y +1), GetIndexFromVerticies(x +1, y));
		}
	}

	//Big ass square
	// AddTriangle(0, 9, 90);
	// AddTriangle(9, 99, 90);

	// VertexColors.Add(FLinearColor(0.f, 0.f, 1.f));
	// VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	// VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	// VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	// VertexColors.Add(FLinearColor(0.5f, 1.f, 0.5f));
	// VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	// VertexColors.Add(FLinearColor(1.f, 1.f, 0.f));
	// VertexColors.Add(FLinearColor(0.f, 1.f, 1.f));

	CustomMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);

	CustomMesh->SetMaterial(0, Material);
}

void AProceduralMeshTest::GenerateCubeMesh()
{
	//6 sides on cube, 4 verts each (corners)

//These are relative locations to the placed Actor in the world
	Vertices.Add(FVector(-100, -100, 0)); //lower left - 0
	Vertices.Add(FVector(-100, -100, 100)); //upper left - 1
	Vertices.Add(FVector(-100, 100, 0)); //lower right - 2 
	Vertices.Add(FVector(-100, 100, 100)); //upper right - 3

	Vertices.Add(FVector(100, -100, 0)); //lower front left - 4
	Vertices.Add(FVector(100, -100, 100)); //upper front left - 5

	Vertices.Add(FVector(100, 100, 100)); //upper front right - 6
	Vertices.Add(FVector(100, 100, 0)); //lower front right - 7

	//Back face of cube
	AddTriangle(0, 2, 3);
	AddTriangle(3, 1, 0);

	//Left face of cube
	AddTriangle(0, 1, 4);
	AddTriangle(4, 1, 5);

	//Front face of cube
	AddTriangle(4, 5, 7);
	AddTriangle(7, 5, 6);

	//Right face of cube
	AddTriangle(7, 6, 3);
	AddTriangle(3, 2, 7);

	//Top face
	AddTriangle(1, 3, 5);
	AddTriangle(6, 5, 3);

	//bottom face
	AddTriangle(2, 0, 4);
	AddTriangle(4, 7, 2);

	TArray<FLinearColor> VertexColors;
	VertexColors.Add(FLinearColor(0.f, 0.f, 1.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.5f, 1.f, 0.5f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 1.f));

	CustomMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);

	CustomMesh->SetMaterial(0, Material);
}

