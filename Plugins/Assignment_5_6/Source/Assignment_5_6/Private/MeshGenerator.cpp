// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshGenerator.h"

// Sets default values
AMeshGenerator::AMeshGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
}

// Called when the game starts or when spawned
void AMeshGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMeshGenerator::BeginDestroy()
{
	Super::BeginDestroy();

	if (AsyncMeshGeneratorTask)
	{
		AsyncMeshGeneratorTask->EnsureCompletion();
		delete AsyncMeshGeneratorTask;
		AsyncMeshGeneratorTask = nullptr;
	}
}

// Called every frame
void AMeshGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeshGenerator::ScatterObjects()
{
	Progress = 1;
	
	for (auto& Pair : HISMComponents) {
		if (UHierarchicalInstancedStaticMeshComponent* HISM = Pair.Value) {
			HISM->ClearInstances();
		}
	}

	if (AsyncMeshGeneratorTask && !AsyncMeshGeneratorTask->IsDone()) {
		AsyncMeshGeneratorTask->EnsureCompletion();
		delete AsyncMeshGeneratorTask;
		AsyncMeshGeneratorTask = nullptr;
	}
	
	AsyncMeshGeneratorTask = new FAsyncTask<FAsyncMeshGeneratorTask>(this);
	AsyncMeshGeneratorTask->StartBackgroundTask();
}

void AMeshGenerator::AddInstances(UStaticMesh* StaticMesh, const TArray<FTransform>& Transforms, UMaterialInterface* Material)
{
	AsyncTask(ENamedThreads::GameThread, [this, StaticMesh, Transforms, Material]()
		{
			if (!this || !IsValid(this)) return;
			UHierarchicalInstancedStaticMeshComponent** HISMCPtr = HISMComponents.Find(StaticMesh);
			if (HISMCPtr && *HISMCPtr && (*HISMCPtr)->IsValidLowLevel())
			{
				if (Material) {
					(*HISMCPtr)->SetMaterial(0, Material);
				}
				(*HISMCPtr)->AddInstances(Transforms, false);
			}
			else
			{
				UHierarchicalInstancedStaticMeshComponent* NewHISMC = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
				NewHISMC->SetStaticMesh(StaticMesh);
				if (Material) {
					NewHISMC->SetMaterial(1, Material);
				}

				HISMComponents.Add(StaticMesh, NewHISMC);

				NewHISMC->RegisterComponentWithWorld(GetWorld());
				NewHISMC->AddInstances(Transforms, false);
			}

			float PercentOfProgress = Progress / float(NoOfMeshInstances);
			UpdateProgressBar(PercentOfProgress);
			Progress++;
		});
}