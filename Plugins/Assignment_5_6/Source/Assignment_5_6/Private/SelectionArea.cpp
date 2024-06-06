// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionArea.h"

// Sets default values
ASelectionArea::ASelectionArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh Component"));
	RootComponent = ProceduralMeshComponent;
}

// Called when the game starts or when spawned
void ASelectionArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASelectionArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int ASelectionArea::AddNewVertex(const FVector& VertexCordinates) {
	return Vertices.Add(VertexCordinates);
}

int ASelectionArea::AddUV(const FVector2D& UVCordinates) {
	return UVs.Add(UVCordinates);
}

void ASelectionArea::DrawTriangleFromVertex(int32 Vertex0, int32 Vertex1, int32 Vertex2) {
	Triangles.Add(Vertex0);
	Triangles.Add(Vertex1);
	Triangles.Add(Vertex2);
}

void ASelectionArea::GenerateCube(const FVector& Dimensions)
{
	Vertices.Reset();
	Triangles.Reset();
	Normals.Reset();
	Normals.Empty();
	UVs.Reset();
	Tangents.Reset();
	Colors.Reset();

	// Bottom Face
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 }); // 0 - - -
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 }); // 1 - + -
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 }); // 2 + - -
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 }); // 3 + + -

	// Top Face
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 }); // 4 - - +
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 }); // 5 - + +
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 }); // 6 + - +
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 }); // 7 + + +

	// Front Face
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 }); // 4 - - - 0
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 }); // 5 - + - 1
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 }); // 6 - - + 4
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 }); // 7 - + + 5

	// Back Face
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 }); // 8 + - - 2
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 }); // 9 + + - 3
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 }); // 10 + - + 6
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 }); // 11 + + + 7

	// Left Face
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 }); // 12 + - - 2
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 }); // 13 - - - 0
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 }); // 14 + - + 6
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 }); // 15 - - + 4

	// Right Face
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 }); // 16 + + - 3
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 }); // 17 - + - 1
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 }); // 18 + + + 7
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 }); // 19 - + + 5

	// Bottom Face Triangle
	DrawTriangleFromVertex(0, 1, 2);
	DrawTriangleFromVertex(1, 3, 2);

	// Top Face Triangle
	DrawTriangleFromVertex(4, 5, 6);
	DrawTriangleFromVertex(5, 7, 6);

	// Front Face Triangle
	DrawTriangleFromVertex(8, 9, 10);
	DrawTriangleFromVertex(9, 11, 10);

	// Back Face Triangle
	DrawTriangleFromVertex(12, 13, 14);
	DrawTriangleFromVertex(13, 15, 14);

	// Left Face Triangle
	DrawTriangleFromVertex(16, 17, 18);
	DrawTriangleFromVertex(17, 19, 18);

	// Right Face Triangle
	DrawTriangleFromVertex(20, 21, 22);
	DrawTriangleFromVertex(21, 23, 22);

	for (int32 i = 0; i < Vertices.Num(); i += 4) {
		AddUV(FVector2D{ 0.0, 1.0 });
		AddUV(FVector2D{ 1.0, 1.0 });
		AddUV(FVector2D{ 0.0, 0.0 });
		AddUV(FVector2D{ 1.0, 0.0 });
	}

	for (int32 i = 0; i < Vertices.Num(); i++) {
		if (i >= 0 && i <= 3) {
			Normals.Add(FVector{ 0.0, 0.0, -1.0 });
		}
		else if (i >= 4 && i <= 7) {
			Normals.Add({ 0.0, 0.0, 1.0 });
		}
		else if (i >= 8 && i <= 11) {
			Normals.Add({ 0.0, -1.0, 0.0 });
		}
		else if (i >= 12 && i <= 15) {
			Normals.Add({ 0.0, 1.0, 0.0 });
		}
		else if (i >= 16 && i <= 19) {
			Normals.Add({ 1.0, 0.0, 0.0 });
		}
		else {
			Normals.Add({ -1.0, 0.0, 0.0 });
		}
	}

	ProceduralMeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
	ProceduralMeshComponent->SetMaterial(0, TranslucentMaterial);
}

void ASelectionArea::GenerateSphere(const float& Radius, const int32& RingCount, const int32& PointCount) {
	Vertices.Reset();
	Triangles.Reset();
	Normals.Reset();
	UVs.Reset();
	Tangents.Reset();
	Colors.Reset();

	for (int32 RingIndex = 0; RingIndex < RingCount; ++RingIndex) {
		float Theta = (PI * RingIndex) / (RingCount - 1);
		float SinTheta = FMath::Sin(Theta);
		float CosTheta = FMath::Cos(Theta);

		float V = Theta / PI;

		for (int32 PointIndex = 0; PointIndex < PointCount; ++PointIndex) {
			float Phi = (2 * PI * PointIndex) / (PointCount - 1);
			float SinPhi = FMath::Sin(Phi);
			float CosPhi = FMath::Cos(Phi);

			float U = Phi / (2 * PI);

			FVector Vertex = FVector{ SinTheta * SinPhi, SinTheta * CosPhi, CosTheta } *Radius;

			Vertices.Add(FVector(Vertex.X, Vertex.Y, Vertex.Z));
			UVs.Add(FVector2D{ U, V });
			Normals.Add(Vertex.GetSafeNormal());

			if (RingIndex < RingCount - 1 && PointIndex < PointCount - 1) {
				int32 CurrentRingVertexIndex = RingIndex * PointCount + PointIndex;
				int32 NextRingVertexIndex = (RingIndex + 1) * PointCount + PointIndex;

				DrawTriangleFromVertex(CurrentRingVertexIndex, NextRingVertexIndex, NextRingVertexIndex + 1);
				DrawTriangleFromVertex(NextRingVertexIndex + 1, CurrentRingVertexIndex + 1, CurrentRingVertexIndex);
			}
		}
	}
	ProceduralMeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
	ProceduralMeshComponent->SetMaterial(0, TranslucentMaterial);
}
