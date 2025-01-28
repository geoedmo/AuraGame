// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageAbility.h"
#include "AuraProjectileSpell.generated.h"


/**
 * 
 */

class AAuraProjectile;
class UGameplayEffect;
struct FGameplayTag;
class AAuraProjectile;
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamageAbility
{
	GENERATED_BODY()
public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpread = 90.f;
	
	UPROPERTY(EditDefaultsOnly)
	float HomingAccelerationMax = 1600.f;

	UPROPERTY(EditDefaultsOnly)
	float HomingAccelerationMin = 3200.f;

	UPROPERTY(EditDefaultsOnly)
	bool bLaunchHomingProjectiles = true;

	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 5;
		
	UPROPERTY(EditDefaultsOnly)
	int32 MaxNumProjectiles = 5;
	

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")	
	virtual void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, TSubclassOf<AAuraProjectile> ProjectileType);

	UFUNCTION(BlueprintCallable)
	virtual void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, AActor* HomingTarget, bool bOverridePitch, float PitchOverride);

};
