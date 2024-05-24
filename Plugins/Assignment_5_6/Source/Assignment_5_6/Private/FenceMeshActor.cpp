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

	SplineComponent->ClearSplinePoints();
}

// Called when the game starts or when spawned
void AFenceMeshActor::BeginPlay()
{
	Super::BeginPlay();
	isProceduralMesh = true ;

	deleteComponents();
	for (int i = 0; i < NoOfSplinePoints; i++)
	{
		FVector Location;
		Location = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		SpawnVerticalMeshActor(Location);
	}
	float Length = (((FencePropeties.spacing * (NoOfSplinePoints - 1)) * FencePropeties.length) + (FencePropeties.length * 50)) / 1500;
	float Height = (FencePropeties.height * 0.075)/100;
	float Width = FencePropeties.width * 0.5 /100 ;

	float RelativeX = (FencePropeties.spacing * (NoOfSplinePoints - 1)) / 2 ;
	float RelativeY = (-0.15 * FencePropeties.width) / 2 + (-0.075 * FencePropeties.width) / 2;
	float RelativeZ = FencePropeties.height * 0.25;

	FActorSpawnParameters SpawnParameters; 
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; 
	
	AVerticalRailActor* VerticalMeshActor_1 = GetWorld()->SpawnActor<AVerticalRailActor>(AVerticalRailActor::StaticClass(), GetActorLocation() + (FVector(RelativeX, RelativeY, RelativeZ*2)), FRotator::ZeroRotator, SpawnParameters);
	VerticalMeshActor_1->SetActorScale3D(FVector(Length, Width, Height));

	AVerticalRailActor* VerticalMeshActor_2 = GetWorld()->SpawnActor<AVerticalRailActor>(AVerticalRailActor::StaticClass(), GetActorLocation() + (FVector(RelativeX, RelativeY, (-RelativeZ / 0.25)*0.40)), FRotator::ZeroRotator, SpawnParameters);
	VerticalMeshActor_2->SetActorScale3D(FVector(Length, Width, Height));
}

void AFenceMeshActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	GenerateFence();
}


// Called every frame
void AFenceMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFenceMeshActor::deleteComponents()
{
	for (int i = 0; i < StaticMeshComponentArr.Num(); i++)
	{
		if (StaticMeshComponentArr[i])
		{
			StaticMeshComponentArr[i]->DestroyComponent();
			StaticMeshComponentArr[i] = nullptr;
		}
	} 
	for (int i = 0; i < H_StaticMeshComponentArr.Num(); i++) {
		if (H_StaticMeshComponentArr[i]) {

			H_StaticMeshComponentArr[i]->DestroyComponent();
			H_StaticMeshComponentArr[i] = nullptr;
		}
	}
	if(!isProceduralMesh){SplineComponent->ClearSplinePoints();}
	StaticMeshComponentArr.Empty();
	H_StaticMeshComponentArr.Empty();
}

void AFenceMeshActor::GenerateFence()
{
	deleteComponents();

	for (int iIndex = 0; iIndex < NoOfSplinePoints; iIndex++)
	{
		SplineComponent->AddSplinePoint(GetActorLocation() + FVector(iIndex * FencePropeties.spacing, 0, 0), ESplineCoordinateSpace::World, false);
			FVector StartPosition, StartTangent;
			SplineComponent->GetLocationAndTangentAtSplinePoint(iIndex, StartPosition, StartTangent, ESplineCoordinateSpace::Local);

			FVector EndPosition, EndTangent;
			SplineComponent->GetLocationAndTangentAtSplinePoint(iIndex + 1, EndPosition, EndTangent, ESplineCoordinateSpace::Local);

			//USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
			UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(this);
			Mesh->SetRelativeScale3D(FVector(FencePropeties.length, FencePropeties.width, FencePropeties.height) / 100);
			Mesh->RegisterComponent();
			Mesh->SetStaticMesh(StaticMesh);
			/*Mesh->SetStartAndEnd(StartPosition, StartTangent, EndPosition, EndTangent);*/
			Mesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
			Mesh->SetRelativeLocation(StartPosition);

			StaticMeshComponentArr.Add(Mesh);
	}

	float Length = ((FencePropeties.spacing * (NoOfSplinePoints - 1)) + (FencePropeties.length * 0.50))/100;
	float Width = (0.15 * FencePropeties.width) / 200;
	float Height = 0.15 * FencePropeties.height/100;

	float RelativeX = (FencePropeties.spacing * (NoOfSplinePoints - 1)) / 2;
	float RelativeY = (-0.15 * FencePropeties.width) / 2 + (-0.075 * FencePropeties.width) / 2;
	float RelativeZ = FencePropeties.height * 0.45;

	UStaticMeshComponent* HorizontalFence_1 = NewObject<UStaticMeshComponent>(this);
	HorizontalFence_1->RegisterComponent();
	HorizontalFence_1->SetWorldScale3D(FVector(Length,Width ,Height));
	HorizontalFence_1->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HorizontalFence_1->SetRelativeLocation(FVector(RelativeX, RelativeY, RelativeZ)); 
	HorizontalFence_1->SetStaticMesh(HorizontalFenceStaticMesh);
	H_StaticMeshComponentArr.Add(HorizontalFence_1);

	UStaticMeshComponent* HorizontalFence_2 = NewObject<UStaticMeshComponent>(this);
	HorizontalFence_2->RegisterComponent();
	HorizontalFence_2->SetWorldScale3D(FVector(Length, Width, Height));
	HorizontalFence_2->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
	HorizontalFence_2->SetRelativeLocation(FVector(RelativeX, RelativeY, (RelativeZ * -0.75)/0.45));
	HorizontalFence_2->SetStaticMesh(HorizontalFenceStaticMesh);
	H_StaticMeshComponentArr.Add(HorizontalFence_2);
}

void AFenceMeshActor::SpawnVerticalMeshActor(const FVector& Location)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (VerticalRailClassRef) {
		AVerticalRailActor* VerticalMeshActor = GetWorld()->SpawnActor<AVerticalRailActor>(VerticalRailClassRef, Location, FRotator::ZeroRotator, SpawnParameters);
		VerticalMeshActor->SetActorScale3D(FVector(FencePropeties.length, FencePropeties.width, FencePropeties.height) / 100);
		VerticalMeshActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}

}