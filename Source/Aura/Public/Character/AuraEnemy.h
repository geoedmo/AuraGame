// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/EnemyInteraction.h"
#include "Character/AuraCharacterBase.h"
#include "UI/WidgetController/AuraOverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */


class UWidgetComponent;
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInteraction
{
	GENERATED_BODY()
	

public:

	AAuraEnemy();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 250.f;

	/* <EnemyInteraction> */

	// ** Overrides ** //
	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;

	/* </EnemyInteraction> */

	/** <Combat Interface> **/
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;
	/** </Combat Interface> **/

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> EnemyHealthBar;

	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;


private:

};
