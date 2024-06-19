// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"
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

	GEngine->AddOnScreenDebugMessage(
		-1, 
		10.f, 
		FColor::Red, 
		FString::Printf(TEXT("Tag: %s"), *GameplayTags.Attributes_Secondary_Armor.ToString())
	);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{	


	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);

}
