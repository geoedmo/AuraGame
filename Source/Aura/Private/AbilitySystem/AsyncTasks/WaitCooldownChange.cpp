// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"
#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag)
{

	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	// want to do something when there is a cooldown effect added, and when it is removed.

	// check for if there is a cooldown tag added.
	
	
	// similiar to the hit react tag changed function!
	// this is used to know when a cooldown has ended.
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InCooldownTag,
		EGameplayTagEventType::NewOrRemoved
	).AddUObject(
			WaitCooldownChange,
			&UWaitCooldownChange::CooldownTagChanged
	);

	// Called when a GE is applied to self (start of cooldown)
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded);

	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid(ASC)) return;

	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();

}

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{

	// The cooldown is over, broadcast
	if (NewCount == 0) {
		CooldownEnd.Broadcast(0.f);
	}

}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{

	FGameplayTagContainer AssetTags;

	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag)) {


		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);

		if (TimesRemaining.Num() > 0) {

			int32 Index = 0;

			float HighestTime = TimesRemaining[0];

			for (int32 i = 0; i < TimesRemaining.Num(); i++) {
				
				if (TimesRemaining[i] > HighestTime)
				{
					HighestTime = TimesRemaining[i];
					Index = i;// Dont really need this, but im keeping it.
				}
			
			}

			float TimeRemaining = HighestTime;


			// now we broadcast.
			
			CooldownStart.Broadcast(TimeRemaining);

		}

	}

}
