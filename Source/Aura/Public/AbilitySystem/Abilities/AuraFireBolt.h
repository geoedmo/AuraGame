// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
public:

	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, AActor* HomingTarget, bool bOverridePitch, float PitchOverride);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpread = 90.f;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxNumProjectiles = 5;

	UPROPERTY(EditDefaultsOnly)
	float HomingAccelerationMax = 1600.f;

	UPROPERTY(EditDefaultsOnly)
	float HomingAccelerationMin = 3200.f;

	UPROPERTY(EditDefaultsOnly)
	bool bLaunchHomingProjectiles = true;

};
