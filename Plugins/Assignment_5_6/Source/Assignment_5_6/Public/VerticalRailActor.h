// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "VerticalRailActor.generated.h"

UENUM()
enum class ERailingType {
	Nothing,
	WindsorTurnedCapital,
	RoundTurnedCapital,
	AcornCapital,
	PyramidTop,
};

UCLASS()
class ASSIGNMENT_5_6_API AVerticalRailActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UProceduralMeshComponent* ProceduralMeshComponent;
	
public:	
	// Sets default values for this actor's properties
	AVerticalRailActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProceduralMeshProps")
	TArray<FVector> Vertices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProceduralMeshProps")
	TArray<int32> Triangles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProceduralMeshProps")
	TArray<FVector> Normals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProceduralMeshProps")
	TArray<FVector2D> UVs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProceduralMeshProps")
	TArray<FProcMeshTangent> Tangents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProceduralMeshProps")
	TArray<FLinearColor> Colors; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProceduralMeshProps")
	ERailingType Railing;

	UPROPERTY()
	int32 SectionIdx;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& FTransform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	int AddNewVertex(const FVector& VertexCordinates);

	// TOP PYRAMID
	UFUNCTION()
	int AddNewVertexCubePyramid(const FVector& VertexCordinates);

	UFUNCTION()
	int AddUV(const FVector2D& UVCordinates);

	UFUNCTION()
	void DrawTriangleFromVertex(int32 Vertex0, int32 Vertex1, int32 Vertex2);

	UFUNCTION(BlueprintCallable, Category = "Defaults")
	void GenerateFenceRailing(ERailingType& FenceRailing);

	UFUNCTION(BlueprintCallable, Category = "Defaults")
	void GenerateCube(int32& SectionIndex, const FVector& Dimesions, const FVector& LocationOffset);

	UFUNCTION(BlueprintCallable, Category = "Defaults")
	void GenerateSphere(int32& SectionIndex, const float& Radius, const int32& RingCount, const int32& PointCount);

	UFUNCTION(BlueprintCallable, Category = "Defaults")
	void GenerateCubePyramid(int32& SectionIndex, const FVector& Dimesions);

	UFUNCTION(BlueprintCallable, Category = "Defaults")
	void GenerateBellShape(int32& SectionIndex, float BaseRadius, float Height1, float RimRadius, float CurvatureFactor, int NumSlices, int NumStacks);

	UFUNCTION(BlueprintCallable, Category = "Defaults")
	void GenerateCurvedCone(int32& SectionIndex, const int32& NumSegments, const int32& NumRings, const int32& Radius, const int32& Height);
	
	UFUNCTION(BlueprintCallable, Category = "Defaults")
	void GenerateOval(int32& SectionIndex, const float& Radius, const int32& RingCount, const int32& PointCount);
};
