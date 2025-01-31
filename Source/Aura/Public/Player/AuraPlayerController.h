// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/HighlightInterface.h"
#include "AuraPlayerController.generated.h"


class ATravelArrowSystem;
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
class AMagicCircle;

enum class ETargetingStatus : uint8
{
	TargetingEnemy,
	TargetingNonEnemy,
	NotTargeting
};

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController, public IHighlightInterface
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();
	
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<AMagicCircle> MagicCircle;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ATravelArrowSystem> TravelArrow;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATravelArrowSystem> TravelArrowClass;
	
	/*
	 * Magic Circle
	 */
	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial);
	
	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
	
	void UpdateMagicCircleLocation();

	/*
	 * Travel Arrow
	 */
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Travel Arrow")
	FVector ArrowDisplayPoint = FVector::ZeroVector;
	
	UFUNCTION(BlueprintCallable)
	void ShowTravelArrow();
	
	UFUNCTION(BlueprintCallable)
	void HideTravelArrow();
	
	void UpdateTravelArrowLocation(float DeltaTime);
	FVector GetArrowDisplayPoint();

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

	ETargetingStatus TargetingStatus = ETargetingStatus::NotTargeting;
	
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

	TObjectPtr<AActor> LastActor;
	TObjectPtr<AActor> ThisActor;

	static void HighlightActor(AActor* InActor);
	static void UnHighlightActor(AActor* InActor);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> AuraInputConfig;



	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UAuraAbilitySystemComponent* GetASC();



};
