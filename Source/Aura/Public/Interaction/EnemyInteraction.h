// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInteraction.generated.h"

UINTERFACE(MinimalAPI)
class UEnemyInteraction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UWidgetComponent;
class AURA_API IEnemyInteraction
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UWidgetComponent* GetHealthBar();

};
