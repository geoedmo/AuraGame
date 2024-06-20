// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraGameplayTags.h"
void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	// This is set in InitActorAbilityInfo in AuraCharacter.CPP So that this delegate is bound at the appropriate time.


	// Changed this to a lamba expression, haven't removed the comments just because I'm not sure if i'll remember how this used to be 
	// for now.
	OnGameplayEffectAppliedDelegateToSelf.AddLambda(

		[this](UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) {
			FGameplayTagContainer TagContainer;
			EffectSpec.GetAllAssetTags(TagContainer);
			EffectAssetTags.Broadcast(TagContainer);
		}
	
	);
		//AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);


	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

}

void UAuraAbilitySystemComponent::AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{	
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
		
			GiveAbility(AbilitySpec);
		}
	
		//GiveAbilityAndActivateOnce(AbilitySpec);
}
	}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
		if (!InputTag.IsValid()) return;

		GetActivatableAbilities();

		for (auto& AbilitySpec : GetActivatableAbilities())
		{
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{

				AbilitySpecInputPressed(AbilitySpec);
				if (!AbilitySpec.IsActive())
				{

					TryActivateAbility(AbilitySpec.Handle);

				}
			}
		}

}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
		if (!InputTag.IsValid()) return;

		for (auto& AbilitySpec : GetActivatableAbilities())
		{
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				AbilitySpecInputReleased(AbilitySpec);
			}
		}

}


void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{	


	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);

}
