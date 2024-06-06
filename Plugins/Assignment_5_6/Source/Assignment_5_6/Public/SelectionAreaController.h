// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "SelectionArea.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "SelectionAreaController.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_5_6_API ASelectionAreaController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "SelectionAreaController")
	FVector Dimensions{100,100,100};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SelectionAreaController")
	float Radius{50};

	UPROPERTY(VisibleDefaultsOnly,Category = "SelectionAreaController")
	FHitResult HitResult;

	UPROPERTY(EditDefaultsOnly , Category = "SelectionAreaController")
	TSubclassOf<ASelectionArea> SelectionActorRef;

	UPROPERTY(BlueprintReadWrite , Category = "SelectionAreaController")
	ASelectionArea* SelectionActor;

	UPROPERTY(BlueprintReadWrite , Category = "SelectionAreaController")
	FString SelectedBoundingBox{"Box"};

	UFUNCTION(BlueprintCallable , Category = "SelectionAreaController")
	void SelectBoundingShape(const FString& ShapeType);

	UFUNCTION(BlueprintCallable , Category = "SelectionAreaController")
	void ChangeScale(const FString& Property , const float& value);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

public:
	void OnClick();
	bool MoveActor{true};
	
};
