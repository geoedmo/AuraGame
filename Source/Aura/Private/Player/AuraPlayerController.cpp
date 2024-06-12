// Fill out your copyright notice in the Description page of Project Settings.



#include "Player/AuraPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInteraction.h"


AAuraPlayerController::AAuraPlayerController() {
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}


void AAuraPlayerController::BeginPlay()
{

	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);

	Subsystem->AddMappingContext(AuraContext, 0);
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;

	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& ActionValue)
{
	const FVector2D InputAxisVector = ActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{

		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);

	}

}

void AAuraPlayerController::CursorTrace()
{

	FHitResult CursorHit;

	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor; // this is being done before, so logically first
	ThisActor = CursorHit.GetActor(); // this is sort of the "current" actor being highlighted

	/** 
	
	* Line Trace from cursor. There are several scenerios:
	* A. LastActor is null && ThisActor is null
	*	- Do nothing.
	* 
	* B. LastActor is null && ThisActor is valid
	*   - Highlight ThisActor
	* 
	* C. LastActor is valid && ThisActor is null
	*	- Unhighlight LastActor
	* 
	* D. Both Actors are valid, but LastActor != ThisActor (switching enemies)
	*   - Unhighlight LastActor, and Highlight ThisActor
	* 
	* E. Both actors are valid, and are the same
	*	- Do nothing.
	
	**/


	if (LastActor == nullptr) 
	{
		if (ThisActor != nullptr) {
			// Case B

			ThisActor->HighlightActor();

		}
		else 
		{
			// Both are null, Case A
		}
	}
	else // LastActor is Valid 
	{
		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnhighlightActor();

		} 
		else // both Actors are Valid
			
		{
			if (LastActor != ThisActor) {

				LastActor->UnhighlightActor();
				ThisActor->HighlightActor();

			}
			else 
			{
				// Do Nothing, case E
			}
		}
	}

}
