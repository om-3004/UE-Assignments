// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MeshAssetManager.h"
#include "SelectionWidget.h"
#include "DisplayMessage.h"
#include "MyStaticMeshActor.h"
#include "Engine/DataTable.h"

#include "CameraViews/PerspectiveViewPawn.h"
#include "CameraViews/OrthographicViewPawn.h"
#include "CameraViews/IsometricViewPawn.h"

#include "WallSpline.h"

#include "InteractiveArchController.generated.h"

UENUM()
enum class EPawnType {
	PerspectiveView,
	OrthographicView,
	IsometricView
};

USTRUCT(Blueprintable)
struct FPawnDataTable: public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EPawnType PawnType{};

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> PawnClass{};
};

DECLARE_DELEGATE_TwoParams(PrintMessage, const FString&, const FString&)

UCLASS()
class ASSIGNMENT_3_4_API AInteractiveArchController : public APlayerController
{
	GENERATED_BODY()

	APerspectiveViewPawn* PerspectivePawn;
	AOrthographicViewPawn* OrthographicPawn;
	AIsometricViewPawn* IsometricPawn;

	TArray<TSubclassOf<APawn>> PawnReference;

	UPROPERTY(VisibleDefaultsOnly, meta = (AlloPrivateAccess = "true"))
	int8 idx;
	
	UPROPERTY(VisibleDefaultsOnly, meta = (AlloPrivateAccess = "true"))
	AActor* HitActor;

	// WallBuilder Controller
	void BuildWall();
	void BuildNewWall();
	void GoToPreviousWall();
	void GoToNextWall();
	void DeleteSegmentOfWall();
	void UndoLastWall();
	void DeleteEveryWall();

	UPROPERTY()
	int currWall;

	UPROPERTY()
	int32 meshNo;

	UPROPERTY()
	AWallSpline* WallSpline;

	UPROPERTY()
	TArray<AWallSpline*> WallSplineArray;

	PrintMessage delegateMsg;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSwitchInputContext;

	UInputMappingContext* MeshSpawnMappingContext;
	UInputMappingContext* ToggleCameraViewMappingContext;
	UInputMappingContext* WallBuilderMappingContext;

protected:
	AInteractiveArchController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	UMeshAssetManager* MeshAssetManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelectionWidget")
	TSubclassOf<USelectionWidget> SelectionWidgetClassRef;

	UPROPERTY()
	USelectionWidget* SelectionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelectionWidget")
	TSubclassOf<UDisplayMessage> DisplayMessageWidgetClassRef;

	UPROPERTY()
	UDisplayMessage* DisplayMessageWidget;

	UFUNCTION()
	void SpawnMeshFromMeshData(const FMeshData& MeshData);
	UFUNCTION()
	void ApplyMaterial(const FMaterialData& MaterialData);
	UFUNCTION()
	void ApplyTexture(const FTextureData& TextureData);

	UPROPERTY()
	FVector CurrentHitLocation;

	UPROPERTY()
	FVector LastHitLocation;

	UPROPERTY()
	AMyStaticMeshActor* CurrentActor;

	UPROPERTY()
	bool isUIHidden;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void SetupToggleCameraViewInput();
	void SetupWallBuilderInput();
	void SetupMeshSpawnInput();
	void ProcessMouseClick();
	void ToggleVisibility();
	void SwitchCameraView();

public:
	UFUNCTION(BlueprintCallable)
	void ToggleInputContext();

	/*UPROPERTY(BlueprintReadWrite, Category = "InteractiveArchSplineController")
	UDisplayMessage* DisplayWidget;*/

	// WallBuilder Controller
	UFUNCTION(BlueprintImplementableEvent)
	void ShowInViewPort(const FString& InfoMsg, const FString& ErrorMsg);

	UFUNCTION(BlueprintImplementableEvent)
	void AddDisplayWidgetToViewPort();
};
