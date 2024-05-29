// Fill out your copyright notice in the Description page of Project Settings.


#include "FenceMeshActor.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
AFenceMeshActor::AFenceMeshActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isProceduralMesh = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	SplineComponent->SetupAttachment(SceneComponent);

	FenceProperties.length = 15;
	FenceProperties.width = 15;
	FenceProperties.height = 200;
	FenceProperties.spacing = 15;
}

// Called when the game starts or when spawned
void AFenceMeshActor::BeginPlay()
{
	Super::BeginPlay();
	GenerateProceduralMesh();
}

void AFenceMeshActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	DestroyStaticFenceComponents();
	GenerateStaticFence();
}


// Called every frame
void AFenceMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFenceMeshActor::DestroyStaticFenceComponents()
{
	for (auto& MeshComponent : VerticalStaticMeshComponentArr)
	{
		if (IsValid(MeshComponent))
			MeshComponent->DestroyComponent();
	}
	VerticalStaticMeshComponentArr.Empty();

	for (auto& MeshComponent : H_StaticMeshComponentArr)
	{
		if (IsValid(MeshComponent))
			MeshComponent->DestroyComponent();
	}
	H_StaticMeshComponentArr.Empty();
	HorizontalMeshLengthArr.Empty();
}

void AFenceMeshActor::GenerateStaticFence()
{
	const float SplineLength = SplineComponent->GetSplineLength();
	const int32 NoOfSplinePoints = SplineComponent->GetNumberOfSplinePoints();

	for (int index = 0; index < NoOfSplinePoints; ++index) {
		SplineComponent->SetSplinePointType(index, ESplinePointType::Linear);
	}

	const float LengthOfStatticFenceMesh = FenceProperties.length + FenceProperties.spacing;
	const float NoOfStaticFences = SplineLength / LengthOfStatticFenceMesh;

	for (int index{}; index < NoOfStaticFences; ++index) {
		const FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(index * LengthOfStatticFenceMesh, ESplineCoordinateSpace::World);
		const FRotator Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(LengthOfStatticFenceMesh * index, ESplineCoordinateSpace::World);

		UStaticMeshComponent* VerticalStaticMesh = NewObject<UStaticMeshComponent>(this);
		VerticalStaticMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepWorldTransform);
		VerticalStaticMesh->SetWorldLocationAndRotation(Location, Rotation);
		VerticalStaticMesh->SetStaticMesh(StaticMesh);
		VerticalStaticMesh->SetWorldScale3D(FVector(FenceProperties.length / 15, FenceProperties.width / 15, FenceProperties.height / 200));
		VerticalStaticMesh->RegisterComponent();

		VerticalStaticMeshComponentArr.Add(VerticalStaticMesh);
	}

	for (int index = 0; index < NoOfSplinePoints - 1; index++) {
		FVector StartLocation;
		FVector StartTangent;
		SplineComponent->GetLocationAndTangentAtSplinePoint(index, StartLocation, StartTangent, ESplineCoordinateSpace::World);

		FVector EndLocation;
		FVector EndTangent;
		SplineComponent->GetLocationAndTangentAtSplinePoint(index + 1, EndLocation, EndTangent, ESplineCoordinateSpace::World);

		FRotator RotationAt = SplineComponent->GetRotationAtSplinePoint(index, ESplineCoordinateSpace::World);

		float LengthBetweenPoints = FVector::Dist(StartLocation, EndLocation);
		float HorizontalMeshLength = LengthOfStatticFenceMesh * (int(LengthBetweenPoints / LengthOfStatticFenceMesh) + 1);
		HorizontalMeshLengthArr.Add(HorizontalMeshLength);
		HorizontalMeshLengthArr.Add(HorizontalMeshLength);

		FVector MiddleVec = (EndLocation + StartLocation) / 2;

		FVector DirectionVec = EndLocation - StartLocation;
		FVector LeftSide = FVector::CrossProduct(DirectionVec, FVector::DownVector).GetSafeNormal();
		LeftSide *= FenceProperties.width / 2 + FenceProperties.width / 4;
		MiddleVec += LeftSide;

		UStaticMeshComponent* HorizontalFence_1 = NewObject<UStaticMeshComponent>(this);
		HorizontalFence_1->RegisterComponent();
		HorizontalFence_1->SetWorldScale3D(FVector(HorizontalMeshLength, FenceProperties.width / 2, FenceProperties.height * 0.075) / 100);
		HorizontalFence_1->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
		HorizontalFence_1->SetWorldLocation(FVector(MiddleVec.X, MiddleVec.Y, MiddleVec.Z + FenceProperties.height / 4));
		HorizontalFence_1->SetWorldRotation(RotationAt);
		HorizontalFence_1->SetStaticMesh(HorizontalFenceStaticMesh);
		HorizontalFence_1->AddLocalOffset(FVector(-FenceProperties.spacing / 2, 0, 0));
		H_StaticMeshComponentArr.Add(HorizontalFence_1);

		UStaticMeshComponent* HorizontalFence_2 = NewObject<UStaticMeshComponent>(this);
		HorizontalFence_2->RegisterComponent();
		HorizontalFence_2->SetWorldScale3D(FVector(HorizontalMeshLength, FenceProperties.width / 2, FenceProperties.height * 0.075) / 100);
		HorizontalFence_2->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
		HorizontalFence_2->SetWorldLocation(FVector(MiddleVec.X, MiddleVec.Y, MiddleVec.Z - (FenceProperties.height / 4)));
		HorizontalFence_2->SetWorldRotation(RotationAt);
		HorizontalFence_2->SetStaticMesh(HorizontalFenceStaticMesh);
		HorizontalFence_2->AddLocalOffset(FVector(-FenceProperties.spacing / 2, 0, 0));
		H_StaticMeshComponentArr.Add(HorizontalFence_2);
	}
}

void AFenceMeshActor::GenerateProceduralMesh()
{
	for (auto& MeshComponent : VerticalStaticMeshComponentArr)
	{
		if (VerticalRailClassRef)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FVector Location = MeshComponent->GetComponentLocation();
			FRotator Rotation = MeshComponent->GetComponentRotation();
			auto SpawnedRail = GetWorld()->SpawnActor<AVerticalRailActor>(VerticalRailClassRef, Location, Rotation, Params);
			SpawnedRail->GenerateFenceRailing(FenceProperties.length, FenceProperties.width, FenceProperties.height);
			SpawnedRail->RegisterAllComponents();
		}
		MeshComponent->DestroyComponent();
	}
	VerticalStaticMeshComponentArr.Empty();

	int index = 0;

	for (auto& H_MeshComponent : H_StaticMeshComponentArr)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector Location = H_MeshComponent->GetComponentLocation();
		FRotator Rotation = H_MeshComponent->GetComponentRotation();
		auto HorizontalRail = GetWorld()->SpawnActor<AVerticalRailActor>(AVerticalRailActor::StaticClass(), Location, Rotation, Params);
		if (HorizontalMeshLengthArr[index]) {
			HorizontalRail->GenerateFenceRailing(HorizontalMeshLengthArr[index], FenceProperties.width / 2, FenceProperties.height * 0.075);
			index++;
		}
		HorizontalRail->RegisterAllComponents();
		H_MeshComponent->DestroyComponent();
	}
	H_StaticMeshComponentArr.Empty();
	HorizontalMeshLengthArr.Empty();
}
