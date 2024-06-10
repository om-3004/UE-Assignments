// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionAreaController.h"

void ASelectionAreaController::BeginPlay()
{
	Super::BeginPlay();
	if (SelectionActorRef) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		SelectionActor = GetWorld()->SpawnActor<ASelectionArea>(SelectionActorRef, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		SelectionActor->GenerateCube({ 100,100,100 });
	}
	bShowMouseCursor = true;
}

void ASelectionAreaController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (MoveActor) {
		FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, SelectionActor);

		FVector CursorWorldLocation;
		FVector CursorWorldDirection;
		DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
			Location = HitResult.Location;

			FVector NewDimensions = SelectionActor->GetActorScale3D() * ((SelectedBoundingBox == "Box") ? Dimensions : FVector(Radius, Radius, Radius));

			Location.Z += (SelectedBoundingBox == "Box") ? NewDimensions.Z / 2 : NewDimensions.Z;
			SelectionActor->SetActorLocation(Location);
		}
	}
}

void ASelectionAreaController::SelectBoundingShape(const FString& ShapeType)
{
	SelectedBoundingBox = ShapeType;
	if (ShapeType == "Sphere") {
		if (SelectionActor) {
			if(bGenerateMesh) {
				SelectionActor->Destroy();
			}
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			SelectionActor = GetWorld()->SpawnActor<ASelectionArea>(SelectionActorRef, Location + FVector(0, 0, Dimensions.Z/2), FRotator::ZeroRotator, SpawnParams);
			SelectionActor->GenerateSphere(Radius, 32, 32);
		}
	}
	else if (ShapeType == "Box") {
		if (SelectionActor) {
			if(bGenerateMesh) {
				SelectionActor->Destroy();
			}
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			SelectionActor = GetWorld()->SpawnActor<ASelectionArea>(SelectionActorRef, Location + FVector(0, 0, Radius), FRotator::ZeroRotator, SpawnParams);
			SelectionActor->GenerateCube(Dimensions);
		}
	}
}

void ASelectionAreaController::ChangeScale(const FString& Property, const float& Value)
{
	FVector Scale = SelectionActor->GetActorScale3D();
	if (Property == "Radius") {
		SelectionActor->SetActorScale3D(FVector(Value, Value, Value) / Radius);
	}
	else if (Property == "DimensionX") {
		SelectionActor->SetActorScale3D(FVector(Value / Dimensions.X, Scale.Y, Scale.Z));
	}
	else if (Property == "DimensionY") {
		SelectionActor->SetActorScale3D(FVector(Scale.X, Value / Dimensions.Y, Scale.Z));
	}
	else if (Property == "DimensionZ") {
		SelectionActor->SetActorScale3D(FVector(Scale.X, Scale.Y, Value / Dimensions.Z));
	}
	FVector NewDimensions = SelectionActor->GetActorScale3D() * ((SelectedBoundingBox == "Box") ? Dimensions : FVector(Radius, Radius, Radius));

	Location.Z = ((SelectedBoundingBox == "Box") ? NewDimensions.Z / 2 : NewDimensions.Z);
	SelectionActor->SetActorLocation(Location);
}

void ASelectionAreaController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UInputMappingContext* InputMappingContext = NewObject<UInputMappingContext>(this);

	UInputAction* ClickAction = NewObject<UInputAction>(this);
	ClickAction->ValueType = EInputActionValueType::Boolean;
	InputMappingContext->MapKey(ClickAction, EKeys::LeftMouseButton);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Completed, this, &ASelectionAreaController::OnClick);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ASelectionAreaController::OnClick()
{
	if (MoveActor) { MoveActor = false; }
	else { MoveActor = true; }
}

void ASelectionAreaController::GenerateMesh(int32 NoOfMeshes)
{
	if (MeshGeneratorActorRef) {
		MeshGeneratorActor = NewObject<AMeshGenerator>(this, MeshGeneratorActorRef);
	}

	if (MeshGeneratorActor) {
		MeshGeneratorActor->SelectedBoundingBox = SelectedBoundingBox;
		MeshGeneratorActor->Dimensions = (SelectedBoundingBox == "Box" ? Dimensions : FVector(Radius, Radius, Radius));
		MeshGeneratorActor->SelectedActor = SelectionActor;
		MeshGeneratorActor->NoOfMeshInstances = NoOfMeshes;
		MeshGeneratorActor->ScatterObjects();
	}
}
