// Fill out your copyright notice in the Description page of Project Settings.


#include "DisapearingGroundMesh.h"

// Sets default values
ADisapearingGroundMesh::ADisapearingGroundMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	SetRootComponent(CustomMesh);
	CustomMesh->bUseAsyncCooking = true;


}

// Called when the game starts or when spawned
void ADisapearingGroundMesh::BeginPlay()
{
	Super::BeginPlay();
    LoadDataFromStaticMesh();
    PrepareProceduralMeshForSplit();
}

void ADisapearingGroundMesh::ExpandMesh()
{
    for (int32 i = 0; i < Vertices.Num() - 3; i += 3)
    {
        FVector avarage(0, 0, 0);

        for (int32 x = i; x < i + 3; x++)
        {
            avarage += Vertices[x];
        }
        avarage /= 3;
        avarage = avarage.GetSafeNormal();

        for (int32 x = i; x < i + 3; x++)
        {

            Vertices[x] += avarage * disappearSpeed;
        }



    }

    CustomMesh->UpdateMeshSection(0, Vertices, normals, UV0, vertexColors, tangents);
    // CustomMesh->SetMaterial(0, MaterialInstance);
}

void ADisapearingGroundMesh::LoadDataFromStaticMesh()
{
      
        if (meshToCopy)
        {
            FPositionVertexBuffer* vertexPositionBuffer = &meshToCopy->RenderData->LODResources[0].VertexBuffers.PositionVertexBuffer;
            FRawStaticIndexBuffer* indexBuffer = &meshToCopy->RenderData->LODResources[0].IndexBuffer;
            FStaticMeshVertexBuffer* vertexBuffer = &meshToCopy->RenderData->LODResources[0].VertexBuffers.StaticMeshVertexBuffer;
            for (int32 i = 0; i < indexBuffer->GetNumIndices(); i++)
            {
                Triangles.Add(indexBuffer->GetArrayView()[i]);
            }

            if (vertexPositionBuffer)
            {

                const int32 vertexCount = vertexPositionBuffer->GetNumVertices();

                
                for (int32 i = 0; i < vertexCount; i++)
                {
                    Vertices.Add(vertexPositionBuffer->VertexPosition(i));
                    normals.Add(FVector(1, 0, 0));
                    UV0.Add(vertexBuffer->GetVertexUV(i, 0));
                    vertexColors.Add(FColor(100, 100, 100, 100));
                    tangents.Add(FProcMeshTangent(vertexBuffer->VertexTangentX(i).X, vertexBuffer->VertexTangentX(i).Y, vertexBuffer->VertexTangentX(i).Z));

                }
            }
            // CustomMesh->CreateMeshSection(1, Vertices, Triangles, normals, UV0, vertexColors, tangents, true);
            // CustomMesh->SetMaterial(0, MaterialInstance);
            
            // MaterialInstance = meshToCopy->GetMaterial(0);
            // UE_LOG(LogTemp, Warning, TEXT("CREATED LANDSCAPE"));

        }

        ScaleOfTriangles.Init(1.0f, Vertices.Num());

}

void ADisapearingGroundMesh::PrepareProceduralMeshForSplit()
{
    UE_LOG(LogTemp, Warning, TEXT("Begin Loop"));

    TArray<int32> preparedTriangles;
    TArray<FVector> preparedVertices;
    for (int32 i = 0; i < Triangles.Num() - 2; i += 3)
    {
        // UE_LOG(LogTemp, Warning, TEXT("Loop"));

        int32 a = Triangles[i];
        int32 b = Triangles[i + 1];
        int32 c = Triangles[i + 2];

        FVector vA = Vertices[a];
        FVector vB = Vertices[b];
        FVector vC = Vertices[c];

        int32 indexOfVertexA = preparedVertices.Num();
        preparedVertices.Add(vA);
        preparedVertices.Add(vB);
        preparedVertices.Add(vC);

        preparedTriangles.Add(indexOfVertexA);
        preparedTriangles.Add(indexOfVertexA + 1);
        preparedTriangles.Add(indexOfVertexA + 2);

        preparedTriangles.Add(indexOfVertexA + 2);
        preparedTriangles.Add(indexOfVertexA + 1);
        preparedTriangles.Add(indexOfVertexA);

    }
    UE_LOG(LogTemp, Warning, TEXT("Number of vericies %f"), preparedVertices.Num());
    UE_LOG(LogTemp, Warning, TEXT("Number of triangles %f"), preparedTriangles.Num());

    Vertices = preparedVertices;
    VerteciesAtBegining = preparedVertices;
    Triangles = preparedTriangles;


    CustomMesh->CreateMeshSection(0, Vertices, Triangles, normals, UV0, vertexColors, tangents, true);
    CustomMesh->SetMaterial(0, MaterialInstance);
    // CustomMesh->UpdateMeshSection(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>());

}

// Called every frame
void ADisapearingGroundMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    for (int32 i = 0; i < Vertices.Num(); i += 3)
    {
        FVector2D avarage(0, 0);

        for (int32 k = i; k < i + 3; k++)
        {
            avarage += FVector2D(Vertices[k].X, Vertices[k].Y);
        }
        avarage /= 3;

        float distance = FVector2D::Distance(avarage, positionOfPlayer);
        // UE_LOG(LogTemp, Warning, TEXT("distance %f"), distance);

        FVector midEdgeOfTriangle = (Vertices[i + 2] + Vertices[i + 1]) / 2.0f;
        FVector middleOfTriangle = Vertices[i] + ((midEdgeOfTriangle - Vertices[i]) * (2.0f / 3.0f));

        float scaleDifference = 0.01f * triangleScalingSpeed;

        float& scaleOfTrianglesI = ScaleOfTriangles[i];
       

    	if(distance > renderCircleRadius)
    	{
            if (scaleOfTrianglesI > scaleDifference)
                scaleOfTrianglesI -= scaleDifference;
            else
                scaleOfTrianglesI = 0.0f;
    	}
        else 
        {
            if (scaleOfTrianglesI < 1.0f - scaleDifference)
                scaleOfTrianglesI += scaleDifference;
            else
                scaleOfTrianglesI = 1.0f;
        }

        for (int32 k = i; k < i + 3; k++)
        {
            Vertices[k] = (midEdgeOfTriangle - VerteciesAtBegining[k]) * (1.0f - scaleOfTrianglesI) + VerteciesAtBegining[k];
        }
    }

    CustomMesh->UpdateMeshSection(0, Vertices, normals, UV0, vertexColors, tangents);
    // CustomMesh->SetMaterial(0, MaterialInstance);


}

