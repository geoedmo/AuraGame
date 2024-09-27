// Fill out your copyright notice in the Description page of Project Settings.



#include "Player/AuraPlayerController.h"
#include "Input/AuraInputComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SplineComponent.h"
#include "AuraGameplayTags.h"
#include "Interaction/EnemyInteraction.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "Player/AuraPlayerState.h"
#include "UI/Widget/DamageTextComponent.h"
#include "NavigationPath.h"
#include "Interaction/CombatInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Actor/MagicCircle.h"
#include "Components/DecalComponent.h"
#include "Aura/Aura.h"


AAuraPlayerController::AAuraPlayerController() {
	bReplicates = true;
	
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");

}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
	UpdateMagicCircleLocation();
}

void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		
		if (DecalMaterial)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
		}
	}
}

void AAuraPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
		MagicCircle = nullptr;
	}
}

void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
			MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
		
	}
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);

		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);

	}
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;

	if (APawn* ControlledPawn = GetPawn())
	{

		// Find Location Closest to Character on Spline
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);

		ControlledPawn->AddMovementInput(Direction);


		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();

		if (DistanceToDestination <= AutoRunAcceptanceRadius) {
			bAutoRunning = false;

		}

	}
}

void AAuraPlayerController::BeginPlay()
{

	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem) 
	{
	Subsystem->AddMappingContext(AuraContext, 0);
	}

	AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	check(PlayerState);

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


	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	AuraInputComponent->BindAbilityActions(AuraInputConfig, this, &AAuraPlayerController::AbilityInputTagPressed, &AAuraPlayerController::AbilityInputTagReleased, &AAuraPlayerController::AbilityInputTagHeld);
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
	if (MagicCircle)
	{
		GetHitResultUnderCursor(ECC_ExcludePlayers, false, CursorHit);
		return;
	}
	
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		if (LastActor) LastActor->UnhighlightActor();

		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}
	
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	
	if (!CursorHit.bBlockingHit) return;
	
	if (CursorHit.GetActor()->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(CursorHit.GetActor()) && !MagicCircle) {
		
		if (LastActor) LastActor->UnhighlightActor();
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}
	

	LastActor = ThisActor; // this is being done before, so logically first
	ThisActor = CursorHit.GetActor(); // this is sort of the "current" actor being highlighted

	if (LastActor != ThisActor) {

		if (LastActor) LastActor->UnhighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}

}
/*
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
/*
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
*/

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{	
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed)) return;

	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
	bTargeting = ThisActor ? true : false; // Turnary Operator
	bAutoRunning = false;
	}

	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);

}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{

	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased)) return;

	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}

	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}

	if (!bTargeting && !bShiftKeyDown)
	{
		bool bIsCurrentlyInMenus = AuraPlayerState->GetMenuStatus();
		if (bIsCurrentlyInMenus) return;

		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn) {

			//Niagara System
			if (!GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
			{ 
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ClickNiagara, CursorHit.ImpactPoint);
			}


			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();

				for (const FVector& PointLoc : NavPath->PathPoints) {

					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);

				}
				if (NavPath->PathPoints.Num() > 0) {
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				bAutoRunning = true;
				}
			}
	
		}

		FollowTime = 0.f;
		bTargeting = false;
		
	}

}
void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{

	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputHeld)) return;

	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}

	// If we are holding LMB down and Targeting

	if (bTargeting || bShiftKeyDown)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	
	} 
	else {

		bool bIsCurrentlyInMenus = AuraPlayerState->GetMenuStatus();
		if (bIsCurrentlyInMenus) return;

		FollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHit.bBlockingHit) {

			// We got a good hit result for the click to move

			CachedDestination = CursorHit.ImpactPoint;

		}

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();

			ControlledPawn->AddMovementInput(WorldDirection);

		}

	}

}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr) {
		
	AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
		
	}
	return AuraAbilitySystemComponent;
}
