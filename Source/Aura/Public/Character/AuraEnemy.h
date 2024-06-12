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

	virtual void HighlightActor() override;
	virtual void UnhighlightActor() override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = Debug)
	bool bDrawDebugSphere = false;

private:


};
