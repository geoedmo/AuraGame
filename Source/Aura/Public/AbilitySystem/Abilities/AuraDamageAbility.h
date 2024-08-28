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


	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* Target = nullptr);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
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



	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomMontageFromArray(const TArray<FTaggedMontage>& TaggedMontage) const;

};
