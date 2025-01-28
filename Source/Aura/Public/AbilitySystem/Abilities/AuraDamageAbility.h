// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraAbilityTypes.h"
#include "AuraDamageAbility.generated.h"

/**
 * 
 */

struct FTaggedMontage;
UCLASS()
class AURA_API UAuraDamageAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
		AActor* Target = nullptr,
		FVector InRadialDamageOrigin = FVector::ZeroVector,
		bool bOverrideKnockbackDirection = false,
		FVector KnockbackDirectionOverride = FVector::ZeroVector,
		bool bOverrideDeathImpulse = false,
		FVector DeathImpulseDirectionOverride = FVector::ZeroVector,
		bool bOverridePitch = false,
		float PitchOverride = 0.0f) const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff Properties")
	float DebuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff Properties")
	float DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff Properties")
	float DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff Properties")
	float DebuffDuration = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Properties")
	float DeathImpulseMagnitude = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff Properties")
	float KnockbackChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Properties")
	float KnockbackMagnitude = 275.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Properties")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Properties")
	bool bIsAOEDamage = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage Properties")
	float RadialDamageInnerRadius = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage Properties")
	float RadialDamageOuterRadius = 0.f;
	
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomMontageFromArray(const TArray<FTaggedMontage>& TaggedMontage) const;

	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;

};
