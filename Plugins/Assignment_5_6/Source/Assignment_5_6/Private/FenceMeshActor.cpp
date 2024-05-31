// Fill out your copyright notice in the Description page of Project Settings.


#include "FenceMeshActor.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
AFenceMeshActor::AFenceMeshActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	NoOfStaticFences = SplineLength / LengthOfStatticFenceMesh;

	for (int index{}; index < NoOfStaticFences; ++index) {
		const FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(index * LengthOfStatticFenceMesh, ESplineCoordinateSpace::Local);
		const FRotator Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(LengthOfStatticFenceMesh * index, ESplineCoordinateSpace::Local);

		UStaticMeshComponent* VerticalStaticMesh = NewObject<UStaticMeshComponent>(this);
		VerticalStaticMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
		VerticalStaticMesh->SetRelativeLocationAndRotation(Location, Rotation);
		VerticalStaticMesh->SetStaticMesh(StaticMesh);
		VerticalStaticMesh->SetRelativeScale3D(FVector(FenceProperties.length / 15, FenceProperties.width / 15, FenceProperties.height / 200));
		VerticalStaticMesh->RegisterComponent();

		UMaterialInstanceDynamic* VerticalDynamicMaterial = UMaterialInstanceDynamic::Create(VerticalRailMaterial, this);
		if (VerticalDynamicMaterial) {
			float TileX = (FenceProperties.length + FenceProperties.width) / 20.0f;
			float TileY = FenceProperties.height / 50.0f;
			VerticalDynamicMaterial->SetScalarParameterValue("TileX", TileX);
			VerticalDynamicMaterial->SetScalarParameterValue("TileY", TileY);
			VerticalStaticMesh->SetMaterial(0, VerticalDynamicMaterial);
		}

		VerticalStaticMeshComponentArr.Add(VerticalStaticMesh);
	}

	for (int index = 0; index < NoOfSplinePoints - 1; index++) {
		FVector StartLocation;
		FVector StartTangent;
		SplineComponent->GetLocationAndTangentAtSplinePoint(index, StartLocation, StartTangent, ESplineCoordinateSpace::Local);

		FVector EndLocation;
		FVector EndTangent;
		SplineComponent->GetLocationAndTangentAtSplinePoint(index + 1, EndLocation, EndTangent, ESplineCoordinateSpace::Local);

		float LengthBetweenPoints = FVector::Dist(StartLocation, EndLocation);
		float HorizontalMeshLength = LengthOfStatticFenceMesh * (int(LengthBetweenPoints / LengthOfStatticFenceMesh) + 1);
		HorizontalMeshLengthArr.Add(HorizontalMeshLength);
		HorizontalMeshLengthArr.Add(HorizontalMeshLength);

		FRotator RotationInBetween = SplineComponent->GetRotationAtSplinePoint(index, ESplineCoordinateSpace::Local);
		FQuat RotationQuat = FQuat(RotationInBetween);

		FVector UpDirectionVector = RotationQuat.RotateVector(FVector::UpVector);
		FVector ForwardDirectionVector = (EndLocation - StartLocation).GetSafeNormal();
		FVector LeftDirectionVector = FVector::CrossProduct(ForwardDirectionVector, UpDirectionVector);
		FVector DownDirectionVector = FVector::CrossProduct(ForwardDirectionVector, LeftDirectionVector);
		FVector RightDirectionVector = FVector::CrossProduct(ForwardDirectionVector, DownDirectionVector);

		FVector LeftOffset = LeftDirectionVector * (FenceProperties.width / 2 + FenceProperties.width / 4);
		FVector UpOffset = UpDirectionVector * (FenceProperties.height / 4);
		FVector DownOffset = DownDirectionVector * (FenceProperties.height / 4);
		FVector ForwardOffset = ForwardDirectionVector * ((HorizontalMeshLength / 2) - (LengthOfStatticFenceMesh/2));

		FVector HFenceLocation1 = StartLocation + LeftOffset + UpOffset + ForwardOffset;
		FVector HFenceLocation2 = StartLocation + LeftOffset + DownOffset + ForwardOffset;

		UStaticMeshComponent* HorizontalFence_1 = NewObject<UStaticMeshComponent>(this);
		HorizontalFence_1->RegisterComponent();
		HorizontalFence_1->SetRelativeScale3D(FVector(HorizontalMeshLength, FenceProperties.width / 2, FenceProperties.height * 0.075) / 100);
		HorizontalFence_1->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
		HorizontalFence_1->SetRelativeLocation(HFenceLocation1);
		HorizontalFence_1->SetRelativeRotation(RotationInBetween);
		HorizontalFence_1->SetStaticMesh(HorizontalFenceStaticMesh);
		H_StaticMeshComponentArr.Add(HorizontalFence_1);

		UMaterialInstanceDynamic* HorizontalDynamicMaterial = UMaterialInstanceDynamic::Create(HorizontalRailMaterial, this);

		if (HorizontalDynamicMaterial) {
			float TileX = HorizontalMeshLength / (NoOfStaticFences * 4);
			float TileY = (FenceProperties.length + FenceProperties.width) / 20.0f;
			HorizontalDynamicMaterial->SetScalarParameterValue("TileX", TileX);
			HorizontalDynamicMaterial->SetScalarParameterValue("TileY", TileY);
			HorizontalFence_1->SetMaterial(0, HorizontalDynamicMaterial);
		}

		UStaticMeshComponent* HorizontalFence_2 = NewObject<UStaticMeshComponent>(this);
		HorizontalFence_2->RegisterComponent();
		HorizontalFence_2->SetRelativeScale3D(FVector(HorizontalMeshLength, FenceProperties.width / 2, FenceProperties.height * 0.075) / 100);
		HorizontalFence_2->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
		HorizontalFence_2->SetRelativeLocation(HFenceLocation2);
		HorizontalFence_2->SetRelativeRotation(RotationInBetween);
		HorizontalFence_2->SetStaticMesh(HorizontalFenceStaticMesh);
		H_StaticMeshComponentArr.Add(HorizontalFence_2);

		if (HorizontalDynamicMaterial) {
			float TileX = HorizontalMeshLength / (NoOfStaticFences * 4);
			float TileY = (FenceProperties.length + FenceProperties.width) / 20.0f;
			HorizontalDynamicMaterial->SetScalarParameterValue("TileX", TileX);
			HorizontalDynamicMaterial->SetScalarParameterValue("TileY", TileY);
			HorizontalFence_2->SetMaterial(0, HorizontalDynamicMaterial);
		}
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

			UMaterialInstanceDynamic* VerticalDynamicMaterial = UMaterialInstanceDynamic::Create(VerticalRailMaterial, this);
			if (VerticalDynamicMaterial) {
				float TileX = FenceProperties.length / 10.0f;
				float TileY = FenceProperties.height / 50.0f;
				VerticalDynamicMaterial->SetScalarParameterValue("TileX", TileX);
				VerticalDynamicMaterial->SetScalarParameterValue("TileY", TileY);
				SpawnedRail->SetVerticalMaterial(0, VerticalDynamicMaterial);
			}
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
		}

		UMaterialInstanceDynamic* HorizontalDynamicMaterial = UMaterialInstanceDynamic::Create(HorizontalRailMaterial, this);
		if (HorizontalDynamicMaterial) {
			float TileX = HorizontalMeshLengthArr[index] / (NoOfStaticFences * 4);
			float TileY = (FenceProperties.length + FenceProperties.width) / 20.0f;
			HorizontalDynamicMaterial->SetScalarParameterValue("TileX", TileX);
			HorizontalDynamicMaterial->SetScalarParameterValue("TileY", TileY);
			HorizontalRail->SetHorizontalMaterial(HorizontalDynamicMaterial);
		}

		HorizontalRail->RegisterAllComponents();
		H_MeshComponent->DestroyComponent();
	}
	H_StaticMeshComponentArr.Empty();
	HorizontalMeshLengthArr.Empty();
	index++;
}
