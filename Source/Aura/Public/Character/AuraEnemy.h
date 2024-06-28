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

	/* <EnemyInteraction> */

	// ** Overrides ** //
	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;

	/* </EnemyInteraction> */

	/** <Combat Interface> **/
	virtual int32 GetPlayerLevel() override;

	/** </Combat Interface> **/
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> EnemyHealthBar;

	virtual void InitializeDefaultAttributes() const override;
private:

};
