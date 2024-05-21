// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonPawn.h"

// Sets default values
AFirstPersonPawn::AFirstPersonPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCapsuleHalfHeight(90);
	CapsuleComponent->SetCapsuleRadius(40);
	CapsuleComponent->SetEnableGravity(false);
	RootComponent = CapsuleComponent;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	
	FPPPawnAttributes = LoadObject<UPawnAttributeAsset>(this, TEXT("/Script/Assignment_1_2.PawnAttributeAsset'/Assignment_1_2/Assignment2/FPP_Attributes.FPP_Attributes'"));
}

// Called when the game starts or when spawned
void AFirstPersonPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFirstPersonPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFirstPersonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		UInputMappingContext* InputMappingContext = NewObject<UInputMappingContext>();

		UInputAction* MoveAction = NewObject<UInputAction>();
		MoveAction->ValueType = EInputActionValueType::Axis3D;

		UInputModifierSwizzleAxis* YXZSwizzleAxisModifier = NewObject<UInputModifierSwizzleAxis>();
		YXZSwizzleAxisModifier->Order = EInputAxisSwizzle::YXZ;

		UInputModifierSwizzleAxis* ZYXSwizzleAxisModifier = NewObject<UInputModifierSwizzleAxis>();
		ZYXSwizzleAxisModifier->Order = EInputAxisSwizzle::ZYX;

		UInputModifierNegate* MoveActionNegateModifier = NewObject<UInputModifierNegate>();
		MoveActionNegateModifier->bX = true;
		MoveActionNegateModifier->bY = true;
		MoveActionNegateModifier->bZ = true;

		InputMappingContext->MapKey(MoveAction, EKeys::W);

		FEnhancedActionKeyMapping& MoveActionBackwardKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::S);
		MoveActionBackwardKeyMapping.Modifiers.Add(MoveActionNegateModifier);

		FEnhancedActionKeyMapping& MoveActionRightKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::D);
		MoveActionRightKeyMapping.Modifiers.Add(YXZSwizzleAxisModifier);

		FEnhancedActionKeyMapping& MoveActionLeftKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::A);
		MoveActionLeftKeyMapping.Modifiers.Add(YXZSwizzleAxisModifier);
		MoveActionLeftKeyMapping.Modifiers.Add(MoveActionNegateModifier);

		FEnhancedActionKeyMapping& MoveActionUpKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::E);
		MoveActionUpKeyMapping.Modifiers.Add(ZYXSwizzleAxisModifier);

		FEnhancedActionKeyMapping& MoveActionDownKeyMapping = InputMappingContext->MapKey(MoveAction, EKeys::Q);
		MoveActionDownKeyMapping.Modifiers.Add(ZYXSwizzleAxisModifier);
		MoveActionDownKeyMapping.Modifiers.Add(MoveActionNegateModifier);


		UInputAction* LookAction = NewObject<UInputAction>();
		LookAction->ValueType = EInputActionValueType::Axis2D;

		UInputModifierNegate* LookActionMouseNegateModifier = NewObject<UInputModifierNegate>();
		LookActionMouseNegateModifier->bX = false;
		LookActionMouseNegateModifier->bY = true;
		LookActionMouseNegateModifier->bZ = false;

		FEnhancedActionKeyMapping& LookActionMouseMapping = InputMappingContext->MapKey(LookAction, EKeys::Mouse2D);
		LookActionMouseMapping.Modifiers.Add(LookActionMouseNegateModifier);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFirstPersonPawn::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFirstPersonPawn::Look);

		if (const APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}

	}

}

void AFirstPersonPawn::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AFirstPersonPawn::Move(const FInputActionValue& Value)
{
	FVector MovementInput = Value.Get<FVector>();
	FRotator Rotation = Controller->GetControlRotation();

	FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);
	FVector UpDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Z);

	AddMovementInput(ForwardDirection, MovementInput.X);
	AddMovementInput(RightDirection, MovementInput.Y);
	AddMovementInput(UpDirection, MovementInput.Z);
}
