// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraPassiveGameplayAbility.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UAuraPassiveGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//  Bind to Deactivate Delegate
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()))) 
	{
		AuraASC->PassiveAbilityDeactivateDelegate.AddUObject(this, &UAuraPassiveGameplayAbility::RecieveDeactivate);
	}
	
}

void UAuraPassiveGameplayAbility::RecieveDeactivate(const FGameplayTag AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag)) {	

		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

	}

}
