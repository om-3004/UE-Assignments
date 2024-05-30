// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SceneComponent.h"
#include "VerticalRailActor.h"
#include "FenceMeshActor.generated.h"

USTRUCT(BlueprintType)
struct FFenceProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float length;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float spacing;
};

UCLASS()
class ASSIGNMENT_5_6_API AFenceMeshActor : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Fence Modificaiton")
	FFenceProperties FenceProperties;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Fence Modificaiton")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Fence Modificaiton")
	USplineComponent* SplineComponent;
	
	UPROPERTY()
	TArray<UStaticMeshComponent*> VerticalStaticMeshComponentArr;
	
	UPROPERTY()
	TArray<UStaticMeshComponent*> H_StaticMeshComponentArr;

	UPROPERTY()
	TArray<float> HorizontalMeshLengthArr;

	UPROPERTY()
	bool isProceduralMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true" ,ClampMin = 0 , UIMin = 0), Category = "Fence Modificaiton")
	UMaterialInterface* VerticalRailMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true" ,ClampMin = 0 , UIMin = 0), Category = "Fence Modificaiton")
	UMaterialInterface* HorizontalRailMaterial;

	UPROPERTY(EditAnywhere,meta = (AllowPrivateAccess = "true"), Category = "Fence Modificaiton")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Fence Modificaiton")
	UStaticMesh* HorizontalFenceStaticMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform);

public:	
	// Sets default values for this actor's properties
	AFenceMeshActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fence Modification")
	TSubclassOf<AVerticalRailActor> VerticalRailClassRef;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable , Category = "Fence Modificaiton")
	void DestroyStaticFenceComponents();

	UFUNCTION(BlueprintCallable , Category = "Fence Modificaiton")
	void GenerateStaticFence();

	UFUNCTION(BlueprintCallable , Category = "Fence Modificaiton")
	void GenerateProceduralMesh();
};
