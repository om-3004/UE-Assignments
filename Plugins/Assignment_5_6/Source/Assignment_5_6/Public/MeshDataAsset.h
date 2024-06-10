// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FMeshProperties {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshProperties")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshProperties")
	UMaterialInterface* Material;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshProperties")
	float MinimumScale;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshProperties")
	float MaximumScale;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshProperties")
	float MinimumRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshProperties")
	float MaximumRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshProperties", meta = (ClampMin=1, UIMin=1, ClampMax=10, UIMax=10))
	float footPrint;
};

UCLASS()
class ASSIGNMENT_5_6_API UMeshDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshProperties")
	TArray<FMeshProperties> MeshPropertiesArr;
};
