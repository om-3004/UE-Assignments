// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicPawnController.h"

ADynamicPawnController::ADynamicPawnController() : indexOfDataTable{ 0 } {

}

void ADynamicPawnController::Spawn() {
	FVector PawnLocation;
	FRotator PawnRotation;
	if (GetPawn()) {

		PawnLocation = GetPawn()->GetActorLocation();
		PawnRotation = GetPawn()->GetActorRotation();

		GetPawn()->Destroy();
	}

	UDataTable* dataTable = LoadObject<UDataTable>(this, TEXT("/Script/Engine.DataTable'/Game/Blueprints/PawnSelection.PawnSelection'"));
	if (dataTable) {
		TArray<FName> rowNames = dataTable->GetRowNames();

		if (rowNames.Num() == indexOfDataTable) {
			indexOfDataTable = 0;
		}

		FPawnDataTable* dataRow = dataTable->FindRow<FPawnDataTable>(rowNames[indexOfDataTable++], TEXT(""));

		UWorld* World = GetWorld();

		if (dataRow) {

			TSubclassOf<APawn> PawnReference = dataRow->PawnClass;

			if (World) {
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
					Subsystem->ClearAllMappings();
				}


				APawn* SpawnedPawn = World->SpawnActor<APawn>(PawnReference, PawnLocation, PawnRotation, SpawnParams);

				SetupInputComponent();

				if (SpawnedPawn) {
					Possess(SpawnedPawn);

					if (dataRow->PawnType == EPawnType::FirstPerson) {
						DisplayAttributes(Cast<AFirstPersonPawn>(SpawnedPawn)->FPPPawnAttributes);
					}
					else {
						DisplayAttributes(Cast<ABaseCharacter>(SpawnedPawn)->TopDownTPPAttributes);
					}

					if (dataRow->PawnType == EPawnType::TopDown) {
						bShowMouseCursor = true;
						bEnableClickEvents = true;
						bEnableMouseOverEvents = true;
					}
					else {

						bShowMouseCursor = false;
						bEnableClickEvents = false;
						bEnableMouseOverEvents = false;
					}
				}
			}
		}


	}

}

void ADynamicPawnController::BeginPlay()
{
	Super::BeginPlay();
}

void ADynamicPawnController::SetupInputComponent() {
	Super::SetupInputComponent();

	UInputMappingContext* InputMappingContext = NewObject<UInputMappingContext>();

	UInputAction* TogglePawn = NewObject<UInputAction>();
	TogglePawn->ValueType = EInputActionValueType::Boolean;

	InputMappingContext->MapKey(TogglePawn, EKeys::P);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(TogglePawn, ETriggerEvent::Completed, this, &ADynamicPawnController::Spawn);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

}
