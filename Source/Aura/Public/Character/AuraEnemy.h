// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/EnemyInteraction.h"
#include "Character/AuraCharacterBase.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInteraction
{
	GENERATED_BODY()
	

public:

	AAuraEnemy();

	virtual void Tick(float DeltaTime) override;


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

private:

};
