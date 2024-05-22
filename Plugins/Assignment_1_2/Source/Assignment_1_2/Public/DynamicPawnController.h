// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Engine/World.h"
#include "GameFramework/Pawn.h"

#include "PawnAttributeAsset.h"
//#include "../TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "FirstPersonPawn.h"

//#include "PawnPossession/TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "DynamicPawnController.generated.h"

/**
 * 
 */

//UENUM(Blueprintable)
UENUM()
enum class EPawnType2 :uint8 {
	ThirdPerson,
	FirstPerson,
	TopDown
};

USTRUCT(Blueprintable)
struct FPawnDataTable2: public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EPawnType2 PawnType{};

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> PawnClass{};
};

UCLASS()
class ASSIGNMENT_1_2_API ADynamicPawnController : public APlayerController
{
	GENERATED_BODY()

	void Spawn();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	/*UPROPERTY(EditAnywhere)
	UPawnAttributeAsset* PawnAttrtibute;*/

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayAttributes(UPawnAttributeAsset* PawnAttrtibute);

public:
	ADynamicPawnController();

	UPROPERTY()
	int8 indexOfDataTable;
	 
};

