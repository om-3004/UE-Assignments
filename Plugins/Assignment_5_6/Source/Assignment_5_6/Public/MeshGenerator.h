// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshDataAsset.h"
#include "SelectionArea.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "FAsyncMeshGeneratorTask.h"
#include "MeshGenerator.generated.h"

UCLASS()
class ASSIGNMENT_5_6_API AMeshGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeshGenerator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshGenerator")
	UMeshDataAsset* MeshDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshGenerator")
	int32 NoOfMeshInstances;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshGenerator")
	ASelectionArea* SelectedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshGenerator")
	FVector Dimensions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshGenerator")
	FString SelectedBoundingBox;

	UFUNCTION(BlueprintCallable, Category = "MeshGenerator")
	void AddInstances(UStaticMesh* StaticMesh, const TArray<FTransform>& Transforms, UMaterialInterface* Material);

	UFUNCTION(BlueprintCallable, Category = "MeshGenerator")
	void ScatterObjects();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshGenerator")
	float Progress;

	UFUNCTION(BlueprintImplementableEvent, Category = "MeshGenerator")
	void UpdateProgressBar(float PercentageOfProgress);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override; 
	virtual void BeginDestroy() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "MeshGenerator")
	USceneComponent* SceneComponent;

	FAsyncTask<FAsyncMeshGeneratorTask>* AsyncMeshGeneratorTask;

	UPROPERTY()
	TMap<UStaticMesh*, UHierarchicalInstancedStaticMeshComponent*> HISMComponents;
};
