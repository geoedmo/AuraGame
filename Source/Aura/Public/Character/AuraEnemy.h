// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/EnemyInteraction.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/HighlightInterface.h"
#include "UI/WidgetController/AuraOverlayWidgetController.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceieveCastTimeSignature, float, CastTime);

class UAuraAttributeSet;
class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInteraction, public IHighlightInterface
{
	GENERATED_BODY()
	

public:

	AAuraEnemy();

	virtual void PossessedBy(AController* NewController) override;

	virtual void Tick(float DeltaTime) override;


	UPROPERTY(BlueprintAssignable)
	FOnReceieveCastTimeSignature CastTimeDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	/* <EnemyInteraction> */

	// ** Overrides ** //

	/* Highlight Interface */
	virtual void HighlightActor_Implementation() override;
	virtual void UnhighlightActor_Implementation() override;
	/* /Highlight Interface */
	
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;

	/* </EnemyInteraction> */

	/** <Combat Interface> **/
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual void FinishSummoning_Implementation() override;
	/** </Combat Interface> **/

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	virtual void StunTagChanged(const FGameplayTag StunTag, int32 NewCount) override;
	virtual void ReceiveCastDurationFromGameEffect(const FGameplayTag CastTag, int32 NewCount) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> EnemyHealthBar;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;


	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

private:

public:
	UWidgetComponent* GetHealthBar_Implementation() { return EnemyHealthBar; }
};
