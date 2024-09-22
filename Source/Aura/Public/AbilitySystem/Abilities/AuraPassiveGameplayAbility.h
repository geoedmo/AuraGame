// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraPassiveGameplayAbility.generated.h"

/**
 * 
 */
struct FGameplayTag;

UCLASS()
class AURA_API UAuraPassiveGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	

	void RecieveDeactivate(const FGameplayTag AbilityTag);
};
