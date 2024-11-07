// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageAbility.h"
#include "AuraFireBlastAbility.generated.h"

class AAuraFireBall;
/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBlastAbility : public UAuraDamageAbility
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireBall*> SpawnFireBalls();
	
protected:
	UPROPERTY(EditAnywhere)
	int32 StartNumFireBalls = 5;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxNumFireBalls = 11;

	UPROPERTY(EditAnywhere)
	float ProjectileSpread = 190.f;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AAuraFireBall> FireBallClass;
	

};
