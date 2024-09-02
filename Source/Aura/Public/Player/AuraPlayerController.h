// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"


struct FGameplayTag;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class IEnemyInteraction;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;
class AAuraPlayerState;
class UDamageTextComponent;
class UNiagaraSystem;
class UNiagaraComponent;


UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();


	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);


protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagara;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraComponent> ClickNiagaraComp;


	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	//Click to move Variables
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;
	FHitResult CursorHit;
	bool bShiftKeyDown;

	TObjectPtr<AAuraPlayerState> AuraPlayerState;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> AuraContext;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> ShiftAction;

	/** Floating Text Variable **/
	UPROPERTY(EditDefaultsOnly, Category = "Floating Text")
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	
	void AutoRun();
	void Move(const FInputActionValue& ActionValue);
	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };
	void CursorTrace();

	TScriptInterface<IEnemyInteraction> LastActor;
	TScriptInterface <IEnemyInteraction> ThisActor;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> AuraInputConfig;



	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UAuraAbilitySystemComponent* GetASC();



};
