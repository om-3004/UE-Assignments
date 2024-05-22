// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveArchController.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"

#include "Engine/StaticMeshActor.h"

AInteractiveArchController::AInteractiveArchController() : currWall{ 0 }, bSwitchInputContext{ true }
{
	//SelectionWidgetClassRef = LoadObject<USelectionWidget>(this, TEXT("/Script /UMGEditor.WidgetBlueprint'/Assignment_3_4/Assignment4/UMG_SelectionWidget.UMG_SelectionWidget'"));

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	isUIHidden = false;
	idx = 0;

	PawnReference.Add(PerspectivePawn->StaticClass());
	PawnReference.Add(OrthographicPawn->StaticClass());
	PawnReference.Add(IsometricPawn->StaticClass());

	// WallBuilder Controller
	meshNo = 0;
	WallSpline = CreateDefaultSubobject<AWallSpline>(TEXT("Wall Spline 0"));

	WallSpline->SplineStaticMesh = LoadObject<UStaticMesh>(this, TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Architecture/Wall_400x200.Wall_400x200'"));
	WallSplineArray.Empty();
	WallSplineArray.Add(WallSpline);

	bShowMouseCursor = true;
	delegateMsg.BindUFunction(this, FName{ "ShowInViewPort" });
}

void AInteractiveArchController::BeginPlay()
{
	Super::BeginPlay();

	if (SelectionWidgetClassRef)
	{
		SelectionWidget = CreateWidget<USelectionWidget>(this, SelectionWidgetClassRef);
		if (SelectionWidget)
		{
			SelectionWidget->ScrollBox1->OnMeshAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::SpawnMeshFromMeshData);
			SelectionWidget->ScrollBox1->OnMaterialAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::ApplyMaterial);
			SelectionWidget->ScrollBox1->OnTextureAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::ApplyTexture);

			SelectionWidget->ScrollBox2->OnMeshAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::SpawnMeshFromMeshData);
			SelectionWidget->ScrollBox2->OnMaterialAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::ApplyMaterial);
			SelectionWidget->ScrollBox2->OnTextureAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::ApplyTexture);

			SelectionWidget->ScrollBox3->OnMeshAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::SpawnMeshFromMeshData);
			SelectionWidget->ScrollBox3->OnMaterialAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::ApplyMaterial);
			SelectionWidget->ScrollBox3->OnTextureAssetThumbnailSelected.BindUObject(this, &AInteractiveArchController::ApplyTexture);
		}
	}
	
	if (DisplayMessageWidgetClassRef)
	{
		DisplayMessageWidget = CreateWidget<UDisplayMessage>(this, DisplayMessageWidgetClassRef);
	}


	SwitchCameraView();
}

void AInteractiveArchController::SetupInputComponent()
{
	Super::SetupInputComponent();

	SetupToggleCameraViewInput();
	SetupWallBuilderInput();
	SetupMeshSpawnInput();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MeshSpawnMappingContext, 0);
			Subsystem->AddMappingContext(ToggleCameraViewMappingContext, 0);
		}
	}
}

void AInteractiveArchController::SetupToggleCameraViewInput()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		ToggleCameraViewMappingContext = NewObject<UInputMappingContext>();

		UInputAction* PossessPawn = NewObject<UInputAction>();
		PossessPawn->ValueType = EInputActionValueType::Boolean;
		ToggleCameraViewMappingContext->MapKey(PossessPawn, EKeys::P);

		EnhancedInputComponent->BindAction(PossessPawn, ETriggerEvent::Completed, this, &AInteractiveArchController::SwitchCameraView);
	}
}

void AInteractiveArchController::SetupWallBuilderInput()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		WallBuilderMappingContext = NewObject<UInputMappingContext>();

		UInputAction* CreateWall = NewObject<UInputAction>();
		CreateWall->ValueType = EInputActionValueType::Boolean;
		WallBuilderMappingContext->MapKey(CreateWall, EKeys::LeftMouseButton);

		UInputAction* NewWall = NewObject<UInputAction>();
		NewWall->ValueType = EInputActionValueType::Boolean;
		WallBuilderMappingContext->MapKey(NewWall, EKeys::RightMouseButton);

		UInputAction* PreviousWall = NewObject<UInputAction>();
		PreviousWall->ValueType = EInputActionValueType::Boolean;
		WallBuilderMappingContext->MapKey(PreviousWall, EKeys::P);

		UInputAction* NextWall = NewObject<UInputAction>();
		NextWall->ValueType = EInputActionValueType::Boolean;
		WallBuilderMappingContext->MapKey(NextWall, EKeys::N);

		UInputAction* DestroySegmentOfWall = NewObject<UInputAction>();
		DestroySegmentOfWall->ValueType = EInputActionValueType::Boolean;
		WallBuilderMappingContext->MapKey(DestroySegmentOfWall, EKeys::B);

		UInputAction* UndoWall = NewObject<UInputAction>();
		UndoWall->ValueType = EInputActionValueType::Boolean;
		WallBuilderMappingContext->MapKey(UndoWall, EKeys::Z);

		UInputAction* DeleteAllSegmentOfWalls = NewObject<UInputAction>();
		DeleteAllSegmentOfWalls->ValueType = EInputActionValueType::Boolean;
		WallBuilderMappingContext->MapKey(DeleteAllSegmentOfWalls, EKeys::C);

		EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
		EnhancedInputComponent->BindAction(CreateWall, ETriggerEvent::Completed, this, &AInteractiveArchController::BuildWall);
		EnhancedInputComponent->BindAction(NewWall, ETriggerEvent::Completed, this, &AInteractiveArchController::BuildNewWall);
		EnhancedInputComponent->BindAction(PreviousWall, ETriggerEvent::Completed, this, &AInteractiveArchController::GoToPreviousWall);
		EnhancedInputComponent->BindAction(NextWall, ETriggerEvent::Completed, this, &AInteractiveArchController::GoToNextWall);
		EnhancedInputComponent->BindAction(DestroySegmentOfWall, ETriggerEvent::Completed, this, &AInteractiveArchController::DeleteSegmentOfWall);
		EnhancedInputComponent->BindAction(UndoWall, ETriggerEvent::Completed, this, &AInteractiveArchController::UndoLastWall);
		EnhancedInputComponent->BindAction(DeleteAllSegmentOfWalls, ETriggerEvent::Completed, this, &AInteractiveArchController::DeleteEveryWall);
		
	}

}

void AInteractiveArchController::SetupMeshSpawnInput()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		MeshSpawnMappingContext = NewObject<UInputMappingContext>();

		UInputAction* ClickAction = NewObject<UInputAction>();
		ClickAction->ValueType = EInputActionValueType::Boolean;
		MeshSpawnMappingContext->MapKey(ClickAction, EKeys::LeftMouseButton);

		UInputAction* ToggleVisibilityOfWidget = NewObject<UInputAction>();
		ToggleVisibilityOfWidget->ValueType = EInputActionValueType::Boolean;
		MeshSpawnMappingContext->MapKey(ToggleVisibilityOfWidget, EKeys::Tab);

		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::ProcessMouseClick);
		EnhancedInputComponent->BindAction(ToggleVisibilityOfWidget, ETriggerEvent::Completed, this, &AInteractiveArchController::ToggleVisibility);

	}
}

// WallBuilder Controller
void AInteractiveArchController::BuildWall()
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	if (HitResult.bBlockingHit)
	{
		FVector ClickedLocation = HitResult.Location;
		if (WallSplineArray[currWall]) {
			WallSplineArray[currWall]->GenerateWall(ClickedLocation);
		}
	}
}

void AInteractiveArchController::BuildNewWall()
{
	if (WallSplineArray[currWall]->SplineComponent->GetNumberOfSplinePoints() == 0) {
		delegateMsg.Execute(FString{ "" }, FString{ "Create a wall in the existing segment before creating a new segment of walls." });
	}
	else if (currWall < WallSplineArray.Num() - 1) {
		delegateMsg.Execute(FString{ "" }, FString{ "Go to the latest wall before creating a new segment of walls." });
	}
	else {
		FHitResult HitResult;
		GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		if (HitResult.bBlockingHit) {
			currWall++;
			meshNo++;
			FString splineName = "Wall Spline " + FString::FromInt(meshNo);
			WallSpline = NewObject<AWallSpline>(this, AWallSpline::StaticClass(), *splineName);
			WallSpline->SplineStaticMesh = LoadObject<UStaticMesh>(this, TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Architecture/Wall_400x200.Wall_400x200'"));
			WallSplineArray.Add(WallSpline);
			delegateMsg.Execute(FString{ "Created a new wall segment. Click and select 2 points to build first wall." }, FString{ "" });
		}
	}
}

void AInteractiveArchController::GoToPreviousWall()
{
	if (currWall > 0) {
		currWall--;
		delegateMsg.Execute(FString{ "Switched to previous wall segment" }, FString{ "" });
	}
	else {
		delegateMsg.Execute(FString{ "" }, FString{ "Already on the starting wall. Can't go previous from this segment of walls." });
	}
}

void AInteractiveArchController::GoToNextWall()
{
	if (currWall < WallSplineArray.Num() - 1) {
		currWall++;
		delegateMsg.Execute(FString{ "Switched to next wall segment" }, FString{ "" });
	}
	else {
		delegateMsg.Execute(FString{ "" }, FString{ "Already on the latest wall. Can't go next from this segment of walls." });
	}
}

void AInteractiveArchController::DeleteSegmentOfWall()
{
	WallSplineArray[currWall]->SplineComponent->ClearSplinePoints();
	WallSplineArray[currWall]->deleteComponents();
	WallSplineArray[currWall]->Destroy();
	if (currWall != WallSplineArray.Num() - 1) {
		WallSplineArray.RemoveAt(currWall);
		delegateMsg.Execute(FString{ "Destroyed this segment of wall. The next segment of wall is automatically selected." }, FString{ "" });
	}
	else if (currWall == 0 && WallSplineArray.Num() == 1) {
		delegateMsg.Execute(FString{ "Destroyed this segment of wall. The next segment of wall is automatically selected." }, FString{ "" });
	}
	else {
		WallSplineArray.RemoveAt(currWall);
		currWall--;
		delegateMsg.Execute(FString{ "Destroyed this segment of wall. The previous segment of wall is automatically selected." }, FString{ "" });
	}
}

void AInteractiveArchController::UndoLastWall()
{
	if (WallSplineArray.Num() > 0 && currWall >= 0) {
		int32 noOfPts = WallSplineArray[currWall]->SplineComponent->GetNumberOfSplinePoints();

		if (noOfPts > 2) {
			WallSplineArray[currWall]->SplineComponent->RemoveSplinePoint(noOfPts - 1);

			WallSplineArray[currWall]->deleteLastWall();
			WallSplineArray[currWall]->GenerateSplineMeshComponents();
			delegateMsg.Execute(FString{ "Undid creation of the last wall in the current segment of walls." }, FString{ "" });
		}
		else if (noOfPts == 2) {
			WallSplineArray[currWall]->SplineComponent->RemoveSplinePoint(noOfPts - 1);
			WallSplineArray[currWall]->SplineComponent->RemoveSplinePoint(noOfPts - 2);

			WallSplineArray[currWall]->deleteLastWall();
			WallSplineArray[currWall]->GenerateSplineMeshComponents();

			if (WallSplineArray.Num() == 1 && currWall == 0) { // Only 1 wall segment
				delegateMsg.Execute(FString{ "Undid creation of the last wall in the current segment of walls. Created a new wall segment." }, FString{ "" });
			}
			else if (currWall < WallSplineArray.Num() - 1) { // Not latest wall segment
				WallSplineArray.RemoveAt(currWall);

				delegateMsg.Execute(FString{ "Undid creation of the last wall in the current segment of walls. Switched to next wall segment." }, FString{ "" });
			}
			else if (currWall == WallSplineArray.Num() - 1) { // Latest wall segment
				WallSplineArray.RemoveAt(currWall);
				currWall--;

				delegateMsg.Execute(FString{ "Undid creation of the last wall in the current segment of walls. Switched to previous wall segment." }, FString{ "" });
			}
		}
		else {
			delegateMsg.Execute(FString{ "" }, FString{ "Not enough spline points to undo last wall in the current segment of walls." });
		}
	}
	else {
		delegateMsg.Execute(FString{ "" }, FString{ "No walls to undo" });
	}
}

void AInteractiveArchController::DeleteEveryWall()
{

	int32 n = WallSplineArray.Num();
	for (int i = 0; i < n; i++) {
		WallSplineArray[i]->SplineComponent->ClearSplinePoints();
		WallSplineArray[i]->deleteComponents();
		WallSplineArray[i]->Destroy();
		if (i != n - 1) {
			delegateMsg.Execute(FString{ "Destroyed all segments of wall. A new segment of wall is automatically selected." }, FString{ "" });
		}
		else if (i == 0 && n == 1) {
			delegateMsg.Execute(FString{ "Destroyed all segments of wall. A new segment of wall is automatically selected." }, FString{ "" });
		}
		else {
			delegateMsg.Execute(FString{ "Destroyed all segments of wall. A new segment of wall is automatically selected." }, FString{ "" });
		}
	}

	for (int i = 1; i < n; i++) {
		WallSplineArray.RemoveAt(1);
	}
	currWall = 0;

}

// InteractiveArch Controller
void AInteractiveArchController::SpawnMeshFromMeshData(const FMeshData& MeshData)
{
	if (MeshData.StaticMesh)
	{
		FBox BoundingBox = MeshData.StaticMesh->GetBoundingBox();
		FVector MinBounds = BoundingBox.Min;
		FVector MaxBounds = BoundingBox.Max;
		float OffsetZ = MaxBounds.Z;

		if (LastHitLocation == CurrentHitLocation) {
			if (CurrentActor) {
				CurrentActor->Destroy();
			}
		}
		else if (AMyStaticMeshActor* StaticMeshActor = Cast<AMyStaticMeshActor>(HitActor)) {
			CurrentHitLocation = StaticMeshActor->GetActorLocation() - FVector(0, 0, OffsetZ);
			StaticMeshActor->Destroy();
		}
		LastHitLocation = CurrentHitLocation;

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CurrentActor = GetWorld()->SpawnActor<AMyStaticMeshActor>(CurrentHitLocation + FVector(0, 0, OffsetZ), FRotator::ZeroRotator, SpawnParameters);

		if (CurrentActor)
		{
			CurrentActor->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
			CurrentActor->GetStaticMeshComponent()->SetStaticMesh(MeshData.StaticMesh);
		}

		if (GetPawn())
		{
			GetPawn()->SetActorLocation(CurrentHitLocation + FVector{ 0, 0, 100 });
		}
	}
}

void AInteractiveArchController::ApplyMaterial(const FMaterialData& MaterialData)
{
	if (CurrentActor && MaterialData.Material)
	{
		UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialData.Material, this);

		if (DynamicMaterialInstance)
		{
			UStaticMeshComponent* StaticMeshComponent = CurrentActor->GetStaticMeshComponent();
			StaticMeshComponent->SetMaterial(0, MaterialData.Material);
		}

	}
}

void AInteractiveArchController::ApplyTexture(const FTextureData& TextureData)
{
	if (CurrentActor && TextureData.Thumbnail)
	{
		UMaterialInstanceDynamic* MaterialInstance = Cast<UMaterialInstanceDynamic>(CurrentActor->GetStaticMeshComponent()->GetMaterial(0));
		if (!MaterialInstance) {
			MaterialInstance = UMaterialInstanceDynamic::Create(CurrentActor->GetStaticMeshComponent()->GetMaterial(0), this);
		}
		MaterialInstance->SetTextureParameterValue(FName("Texture"), TextureData.Thumbnail);
		CurrentActor->GetStaticMeshComponent()->SetMaterial(0, MaterialInstance);
	}
}

void AInteractiveArchController::ProcessMouseClick()
{
	FVector WorldLocation, WorldDirection;
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		FHitResult HitResult;
		FVector TraceEnd = WorldLocation + WorldDirection * 10000.0f;

		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = true;
		QueryParams.AddIgnoredActor(GetPawn());

		if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, ECC_Visibility, QueryParams))
		{
			HitActor = HitResult.GetActor();
			if (HitActor)
			{
				if (AMyStaticMeshActor* StaticMeshActor = Cast<AMyStaticMeshActor>(HitActor)) {
					SelectionWidget->ScrollBox1->SetVisibility(ESlateVisibility::Visible);
					SelectionWidget->ScrollBox2->SetVisibility(ESlateVisibility::Visible);
					SelectionWidget->ScrollBox3->SetVisibility(ESlateVisibility::Visible);
					CurrentActor = StaticMeshActor;
					if (GetPawn())
					{
						GetPawn()->SetActorLocation(HitActor->GetActorLocation() + FVector{ 0, 0, 50 });
					}
				}
				else {
					if (SelectionWidget->ScrollBox1->TypeOfAsset == EAssetType::MeshData) {
						SelectionWidget->ScrollBox1->SetVisibility(ESlateVisibility::Visible);
						SelectionWidget->ScrollBox2->SetVisibility(ESlateVisibility::Hidden);
						SelectionWidget->ScrollBox3->SetVisibility(ESlateVisibility::Hidden);
					}
					else if (SelectionWidget->ScrollBox2->TypeOfAsset == EAssetType::MeshData) {
						SelectionWidget->ScrollBox2->SetVisibility(ESlateVisibility::Visible);
						SelectionWidget->ScrollBox1->SetVisibility(ESlateVisibility::Hidden);
						SelectionWidget->ScrollBox3->SetVisibility(ESlateVisibility::Hidden);
					}
					else if (SelectionWidget->ScrollBox3->TypeOfAsset == EAssetType::MeshData) {
						SelectionWidget->ScrollBox3->SetVisibility(ESlateVisibility::Visible);
						SelectionWidget->ScrollBox1->SetVisibility(ESlateVisibility::Hidden);
						SelectionWidget->ScrollBox2->SetVisibility(ESlateVisibility::Hidden);
					}
				}
				CurrentHitLocation = HitResult.Location;

				if (SelectionWidget && !SelectionWidget->IsInViewport())
				{
					SelectionWidget->AddToViewport();
				}
			}
		}
	}
}

void AInteractiveArchController::ToggleVisibility()
{
	if (SelectionWidget->IsInViewport())
	{
		if (isUIHidden)
		{
			SelectionWidget->SetVisibility(ESlateVisibility::Visible);
			isUIHidden = false;
		}
		else
		{
			SelectionWidget->SetVisibility(ESlateVisibility::Hidden);
			isUIHidden = true;
		}
	}
}

void AInteractiveArchController::SwitchCameraView()
{
	if (idx == PawnReference.Num())
	{
		idx = 0;
	}

	TSubclassOf<APawn> PawnClass = PawnReference[idx++];
	if (PawnClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		if (GetPawn())
		{
			GetPawn()->Destroy();
		}

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
			Subsystem->ClearAllMappings();
		}

		APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, LastHitLocation + FVector(0, 0, 200), FRotator::ZeroRotator, SpawnParams);

		SetupInputComponent();

		if (SpawnedPawn)
		{
			Possess(SpawnedPawn);
		}
	}
}

void AInteractiveArchController::ToggleInputContext()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (bSwitchInputContext)
		{
			Subsystem->RemoveMappingContext(MeshSpawnMappingContext);
			Subsystem->RemoveMappingContext(ToggleCameraViewMappingContext);
			SelectionWidget->RemoveFromViewport();
			DisplayMessageWidget->AddToViewport();
			AddDisplayWidgetToViewPort();

			if (WallBuilderMappingContext)
			{
				Subsystem->AddMappingContext(WallBuilderMappingContext, 0);
			}
			bSwitchInputContext = false;
		}
		else
		{
			Subsystem->RemoveMappingContext(WallBuilderMappingContext);
			DisplayMessageWidget->RemoveFromViewport();
			SelectionWidget->AddToViewport();

			if (MeshSpawnMappingContext)
			{
				Subsystem->AddMappingContext(MeshSpawnMappingContext, 0);
				Subsystem->AddMappingContext(ToggleCameraViewMappingContext, 0);
			}
			bSwitchInputContext = true;
		}
	}
}
