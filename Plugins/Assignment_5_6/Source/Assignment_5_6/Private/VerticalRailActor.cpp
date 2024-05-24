// Fill out your copyright notice in the Description page of Project Settings.


#include "VerticalRailActor.h"

// Sets default values
AVerticalRailActor::AVerticalRailActor(): SectionIdx{ 0 }
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Railing = ERailingType::Nothing;


	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh Component"));
	RootComponent = ProceduralMeshComponent;

	for (int32 i = 0; i < Vertices.Num(); i++) {
		Colors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

}

// Called when the game starts or when spawned
void AVerticalRailActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVerticalRailActor::OnConstruction(const FTransform& FTransform)
{
	Super::OnConstruction(FTransform);
	GenerateFenceRailing(Railing);
}

// Called every frame
void AVerticalRailActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int AVerticalRailActor::AddNewVertex(const FVector& VertexCordinates) {
	return Vertices.Add(VertexCordinates);
}

int AVerticalRailActor::AddUV(const FVector2D& UVCordinates) {
	return UVs.Add(UVCordinates);
}

void AVerticalRailActor::DrawTriangleFromVertex(int32 Vertex0, int32 Vertex1, int32 Vertex2) {
	Triangles.Add(Vertex0);
	Triangles.Add(Vertex1);
	Triangles.Add(Vertex2);
}

void AVerticalRailActor::GenerateFenceRailing(ERailingType& FenceRailing)
{
	GenerateCube(SectionIdx, { 15, 15, 200 }, { 0, 0, 0 });

	switch (FenceRailing) {
	case ERailingType::WindsorTurnedCapital:
		GenerateBellShape(SectionIdx, 5, 2, 1.5, 1, 10, 10);
		GenerateSphere(SectionIdx, 8, 25, 25);
		GenerateCurvedCone(SectionIdx, 16, 16, 3, 5);
		break;
	case ERailingType::RoundTurnedCapital:
		GenerateBellShape(SectionIdx, 5, 2, 1.5, 1, 10, 10);
		GenerateSphere(SectionIdx, 8, 25, 25);
		break;
	case ERailingType::AcornCapital:
		GenerateBellShape(SectionIdx, 5, 2, 1.5, 1, 10, 10);
		GenerateOval(SectionIdx, 8, 25, 25);
		break;
	case ERailingType::PyramidTop:
		GenerateCube(SectionIdx, { 10, 10, 1 }, { 0, 0, 100.5 });
		GenerateCubePyramid(SectionIdx, {15, 15, 10});
		break;
	}
}

void AVerticalRailActor::GenerateCube(int32& SectionIndex, const FVector& Dimensions, const FVector& LocationOffset)
{
    Vertices.Reset();
	Triangles.Reset(); 
	Normals.Reset(); 
	UVs.Reset(); 
	Tangents.Reset(); 
	Colors.Reset(); 
    
	//ProceduralMeshComponent->ClearAllMeshSections();

	// Bottom Face
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 0 - - -
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 1 - + -
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 2 + - -
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 3 + + -

	// Top Face
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 4 - - +
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 5 - + +
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 6 + - +
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 7 + + +

	// Front Face
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 8 - - - 0
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 9 - + - 1
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 10 - - + 4
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 11 - + + 5

	// Back Face
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 12 + - - 2
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 13 + + - 3
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 14 + - + 6
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 15 + + + 7

	// Left Face
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 16 + - - 2
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 17 - - - 0
	AddNewVertex(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 18 + - + 6
	AddNewVertex(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 19 - - + 4

	// Right Face
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 20 + + - 3
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 21 - + - 1
	AddNewVertex(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 22 + + + 7
	AddNewVertex(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 23 - + + 5

	// Bottom Face Triangle
	DrawTriangleFromVertex(2, 3, 0);
	DrawTriangleFromVertex(3, 1, 0);

	// Top Face Triangle
	DrawTriangleFromVertex(4, 5, 6);
	DrawTriangleFromVertex(5, 7, 6);

	// Front Face Triangle
	DrawTriangleFromVertex(8, 9, 10);
	DrawTriangleFromVertex(9, 11, 10);

	// Back Face Triangle
	DrawTriangleFromVertex(13, 12, 15);
	DrawTriangleFromVertex(12, 14, 15);

	// Left Face Triangle
	DrawTriangleFromVertex(16, 17, 18);
	DrawTriangleFromVertex(17, 19, 18);

	// Right Face Triangle
	DrawTriangleFromVertex(21, 20, 23);
	DrawTriangleFromVertex(20, 22, 23);

	for (int32 i = 0; i < Vertices.Num(); i += 4) {
		AddUV(FVector2D{ 0.0, 1.0 });
		AddUV(FVector2D{ 1.0, 1.0 });
		AddUV(FVector2D{ 0.0, 0.0 });
		AddUV(FVector2D{ 1.0, 0.0 });
	}
	ProceduralMeshComponent->CreateMeshSection_LinearColor(SectionIndex++, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}

void AVerticalRailActor::GenerateSphere(int32& SectionIndex, const float& Radius, const int32& RingCount, const int32& PointCount) {
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

			Vertices.Add(FVector(Vertex.X, Vertex.Y, Vertex.Z + 110));
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
	ProceduralMeshComponent->CreateMeshSection_LinearColor(SectionIndex++, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}

int AVerticalRailActor::AddNewVertexCubePyramid(const FVector& VertexCordinates) {
	return Vertices.Add(VertexCordinates + FVector(0, 0, 106));
}

void AVerticalRailActor::GenerateCubePyramid(int32& SectionIndex, const FVector& Dimensions)
{
	Vertices.Reset();
	Triangles.Reset();
	Normals.Reset();
	UVs.Reset();
	Tangents.Reset();
	Colors.Reset();

	// Bottom Face
	AddNewVertexCubePyramid(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 }); // 0 - - -
	AddNewVertexCubePyramid(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 }); // 1 - + -
	AddNewVertexCubePyramid(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 }); // 2 + - -
	AddNewVertexCubePyramid(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 }); // 3 + + -

	// Front Face
	AddNewVertexCubePyramid(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 }); // 4 - - - 0
	AddNewVertexCubePyramid(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 }); // 5 - + - 1
	AddNewVertexCubePyramid(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 }); // 6 - - + 4
	AddNewVertexCubePyramid(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 }); // 7 - + + 5

	// Back Face
	AddNewVertexCubePyramid(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 }); // 8 + - - 2
	AddNewVertexCubePyramid(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 }); // 9 + + - 3
	AddNewVertexCubePyramid(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 }); // 10 + - + 6
	AddNewVertexCubePyramid(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 }); // 11 + + + 7

	// Left Face
	AddNewVertexCubePyramid(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 }); // 12 + - - 2
	AddNewVertexCubePyramid(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 }); // 13 - - - 0
	AddNewVertexCubePyramid(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 }); // 14 + - + 6
	AddNewVertexCubePyramid(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 }); // 15 - - + 4

	// Right Face
	AddNewVertexCubePyramid(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 }); // 16 + + - 3
	AddNewVertexCubePyramid(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 }); // 17 - + - 1
	AddNewVertexCubePyramid(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 }); // 18 + + + 7
	AddNewVertexCubePyramid(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 }); // 19 - + + 5

	// Triangle Front
	AddNewVertexCubePyramid(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 }); // 20
	AddNewVertexCubePyramid(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 }); // 21
	AddNewVertexCubePyramid(FVector{ 0, 0, Dimensions.Z }); // 22 

	// Triangle Back
	AddNewVertexCubePyramid(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 }); // 23
	AddNewVertexCubePyramid(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 }); // 24
	AddNewVertexCubePyramid(FVector{ 0, 0, Dimensions.Z }); // 25

	// Triangle Left
	AddNewVertexCubePyramid(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 }); // 26
	AddNewVertexCubePyramid(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 }); // 27
	AddNewVertexCubePyramid(FVector{ 0, 0, Dimensions.Z }); // 28

	// Triangle Right
	AddNewVertexCubePyramid(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 }); // 29
	AddNewVertexCubePyramid(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 }); // 30
	AddNewVertexCubePyramid(FVector{ 0, 0, Dimensions.Z }); // 31

	// Bottom Face Triangle
	DrawTriangleFromVertex(2, 3, 0);
	DrawTriangleFromVertex(3, 1, 0);

	// Front Face Triangle
	DrawTriangleFromVertex(4, 5, 6);
	DrawTriangleFromVertex(5, 7, 6);

	// Back Face Triangle
	DrawTriangleFromVertex(8, 9, 10);
	DrawTriangleFromVertex(9, 11, 10);

	// Left Face Triangle
	DrawTriangleFromVertex(12, 13, 14);
	DrawTriangleFromVertex(13, 15, 14);

	// Right Face Triangle
	DrawTriangleFromVertex(16, 17, 18);
	DrawTriangleFromVertex(17, 19, 18);

	// Triangle Front
	DrawTriangleFromVertex(20, 21, 22);

	// Triangle Back
	DrawTriangleFromVertex(23, 24, 25);

	// Triangle Left
	DrawTriangleFromVertex(26, 27, 28);

	// Triangle Right
	DrawTriangleFromVertex(29, 30, 31);

	for (int32 i = 0; i < 20; i += 4) {
		AddUV(FVector2D{ 0.0, 1.0 });
		AddUV(FVector2D{ 1.0, 1.0 });
		AddUV(FVector2D{ 0.0, 0.0 });
		AddUV(FVector2D{ 1.0, 0.0 });
	}

	for (int32 i = 20; i < 32; i += 3) {
		AddUV(FVector2D{ 0.0, 0.0 });
		AddUV(FVector2D{ 1.0, 0.0 });
		AddUV(FVector2D{ 0.0, 1.0 });
	}

	Normals.Add({ 0.0, 0.0, -1.0 });
	Normals.Add({ -1.0, 0.0, 0.0 });
	Normals.Add({ 1.0, 0.0, 0.0 });
	Normals.Add({ 0.0, -1.0, 0.0 });
	Normals.Add({ 0.0, 1.0, 0.0 });

	Normals.Add(FVector::CrossProduct(Vertices[20], Vertices[21]));
	Normals.Add(FVector::CrossProduct(Vertices[23], Vertices[24]));
	Normals.Add(FVector::CrossProduct(Vertices[26], Vertices[27]));
	Normals.Add(FVector::CrossProduct(Vertices[29], Vertices[30]));

	ProceduralMeshComponent->CreateMeshSection_LinearColor(SectionIndex++, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}

void AVerticalRailActor::GenerateBellShape(int32& SectionIndex, float BaseRadius, float Height1, float RimRadius, float CurvatureFactor, int NumSlices, int NumStacks)
{
	Vertices.Reset();
	Triangles.Reset();
	Normals.Reset();
	UVs.Reset();
	Tangents.Reset();
	Colors.Reset();

	float Theta, Phi;
	float DeltaTheta = 2.0f * PI / NumSlices;
	float DeltaPhi = PI / (NumStacks - 1);

	for (int i = 0; i < NumStacks; i++)
	{
		Phi = i * DeltaPhi;

		float t = Phi / (PI / 2.0f);
		float CurrentRadius = FMath::Lerp(BaseRadius, RimRadius, FMath::Sin(t * CurvatureFactor)) + 2.5;

		for (int j = 0; j < NumSlices; j++)
		{
			Theta = j * DeltaTheta;

			float X = CurrentRadius * FMath::Sin(Theta);
			float Y = CurrentRadius * FMath::Cos(Theta);
			float Z = Height1 * (t - 1.0f);

			Z += 100;

			Vertices.Add(FVector(X, Y, Z + Height1));
			Normals.Add(FVector(X, Y, Z).GetSafeNormal());
			UVs.Add(FVector2D((float)j / (NumSlices - 1), (float)i / (NumStacks - 1)));
			Tangents.Add(FProcMeshTangent(-FMath::Sin(Theta), FMath::Cos(Theta), 0.0f));
		}
	}

	for (int i = 0; i < NumStacks - 1; i++)
	{
		for (int j = 0; j < NumSlices - 1; j++)
		{
			int BottomLeft = i * NumSlices + j;
			int BottomRight = BottomLeft + 1;
			int TopLeft = BottomLeft + NumSlices;
			int TopRight = TopLeft + 1;

			Triangles.Add(BottomLeft);
			Triangles.Add(TopLeft);
			Triangles.Add(BottomRight);

			Triangles.Add(BottomLeft);
			Triangles.Add(BottomRight);
			Triangles.Add(TopLeft);

			Triangles.Add(TopLeft);
			Triangles.Add(TopRight);
			Triangles.Add(BottomRight);

			Triangles.Add(TopLeft);
			Triangles.Add(BottomRight);
			Triangles.Add(TopRight);
		}

		int BottomLeftWrap = i * NumSlices + (NumSlices - 1);
		int BottomRightWrap = i * NumSlices;
		int TopLeftWrap = BottomLeftWrap + NumSlices;
		int TopRightWrap = BottomRightWrap + NumSlices;

		Triangles.Add(BottomLeftWrap);
		Triangles.Add(TopLeftWrap);
		Triangles.Add(BottomRightWrap);

		Triangles.Add(BottomLeftWrap);
		Triangles.Add(BottomRightWrap);
		Triangles.Add(TopLeftWrap);

		Triangles.Add(TopLeftWrap);
		Triangles.Add(TopRightWrap);
		Triangles.Add(BottomRightWrap);

		Triangles.Add(TopLeftWrap);
		Triangles.Add(BottomRightWrap);
		Triangles.Add(TopRightWrap);
	}

	ProceduralMeshComponent->CreateMeshSection_LinearColor(SectionIndex++, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}

void AVerticalRailActor::GenerateCurvedCone(int32& SectionIndex, const int32& NumSegments, const int32& NumRings, const int32& Radius, const int32& Height) {
	Vertices.Reset();
	Triangles.Reset();
	Normals.Reset();
	UVs.Reset();
	Tangents.Reset();
	Colors.Reset();

	for (int32 Ring = 0; Ring <= NumRings; ++Ring)
	{
		float RingFraction = (float)Ring / (float)NumRings;
		float Z = Height * (1.0f - RingFraction);
		float RingRadius = Radius * FMath::Sin(RingFraction * PI * 0.5f);

		if (Ring > NumRings * 0.8f) {
			RingRadius *= 0.5f + 0.5f * FMath::Sin((RingFraction - 0.8f) * PI * 2.5f);
		}

		for (int32 Segment = 0; Segment <= NumSegments; ++Segment)
		{
			float SegmentFraction = (float)Segment / (float)NumSegments;
			float Theta = SegmentFraction * 2.0f * PI;
			float X = RingRadius * FMath::Cos(Theta);
			float Y = RingRadius * FMath::Sin(Theta);

			Vertices.Add(FVector(X, Y, Z + 116.3));
			Normals.Add(FVector(X, Y, 0).GetSafeNormal());
			UVs.Add(FVector2D(SegmentFraction, RingFraction));
			Colors.Add(FLinearColor::White);
			Tangents.Add(FProcMeshTangent(-FMath::Sin(Theta), FMath::Cos(Theta), 0));
		}
	}

	for (int32 Ring = 0; Ring < NumRings; ++Ring)
	{
		for (int32 Segment = 0; Segment < NumSegments; ++Segment)
		{
			int32 A = Ring * (NumSegments + 1) + Segment;
			int32 B = (Ring + 1) * (NumSegments + 1) + Segment;
			int32 C = A + 1;
			int32 D = B + 1;

			Triangles.Add(C);
			Triangles.Add(B);
			Triangles.Add(A);

			Triangles.Add(C);
			Triangles.Add(D);
			Triangles.Add(B);
		}
	}

	ProceduralMeshComponent->CreateMeshSection_LinearColor(SectionIndex++, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}

void AVerticalRailActor::GenerateOval(int32& SectionIndex, const float& Radius, const int32& RingCount, const int32& PointCount) {
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

			FVector Vertex = FVector{ SinTheta * SinPhi * Radius, SinTheta * CosPhi * Radius, CosTheta * Radius * 1.5};

			Vertices.Add(FVector(Vertex.X, Vertex.Y, Vertex.Z + 114));
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
	ProceduralMeshComponent->CreateMeshSection_LinearColor(SectionIndex++, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}