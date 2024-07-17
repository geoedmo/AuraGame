// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraDamageAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"


void UAuraDamageAbility::CauseDamage(AActor* TargetActor)
{

	FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);

	for (TTuple<FGameplayTag, FScalableFloat> Pair : DamageTypes)
	{

		const float DamageMagnitude = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageEffectSpecHandle, Pair.Key, DamageMagnitude);

	}

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FTaggedMontage UAuraDamageAbility::GetRandomMontageFromArray(const TArray<FTaggedMontage>& TaggedMontage) const
{
	if (TaggedMontage.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontage.Num() - 1);
		return TaggedMontage[Selection];

	}
	return FTaggedMontage();
}
