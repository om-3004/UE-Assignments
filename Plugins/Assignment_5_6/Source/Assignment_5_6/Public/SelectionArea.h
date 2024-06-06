// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ProceduralMeshComponent.h"

#include "SelectionArea.generated.h"

UCLASS()
class ASSIGNMENT_5_6_API ASelectionArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectionArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable , Category = "SelectionActor")
	void GenerateCube(const FVector& Dimensions);

	UFUNCTION(BlueprintCallable , Category = "SelectionActor")
	void GenerateSphere(const float& Radius, const int32& RingCount, const int32& PointCount);

	UPROPERTY(VisibleDefaultsOnly, Category = "SelectionActor")
	UProceduralMeshComponent* ProceduralMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelectionActor")
	UMaterialInterface* TranslucentMaterial;
private:

	UPROPERTY(VisibleDefaultsOnly, Category = "SelectionActor")
	TArray<FVector> Vertices;

	UPROPERTY(VisibleDefaultsOnly, Category = "SelectionActor")
	TArray<int32> Triangles;

	UPROPERTY(VisibleDefaultsOnly, Category = "SelectionActor")
	TArray<FVector> Normals;

	UPROPERTY(VisibleDefaultsOnly, Category = "SelectionActor")
	TArray<FVector2D> UVs;

	UPROPERTY(VisibleDefaultsOnly, Category = "SelectionActor")
	TArray<FProcMeshTangent> Tangents;

	UPROPERTY(VisibleDefaultsOnly, Category = "SelectionActor")
	TArray<FLinearColor> Colors; 

	int AddNewVertex(const FVector& VertexCordinates);

	int AddUV(const FVector2D& UVCordinates);

	void DrawTriangleFromVertex(int32 Vertex0, int32 Vertex1, int32 Vertex2);
};
