// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/WaitCastTimeChange.h"
#include "AuraGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"

UWaitCastTimeChange::UWaitCastTimeChange()
{

}

UWaitCastTimeChange* UWaitCastTimeChange::WaitForCastCompletion(UAbilitySystemComponent* AbilitySystemComponent)
{
	UWaitCastTimeChange* WaitCastTimeChange = NewObject<UWaitCastTimeChange>();
	WaitCastTimeChange->ASC = AbilitySystemComponent;

	if (!IsValid(AbilitySystemComponent))
	{
		WaitCastTimeChange->EndTask();
		return nullptr;
	}

	// want to do something when there is a cooldown effect added, and when it is removed.

	// check for if there is a cooldown tag added.


	// similiar to the hit react tag changed function!
	// this is used to know when a cooldown has ended.
	AbilitySystemComponent->RegisterGameplayTagEvent(
		FAuraGameplayTags::Get().Player_Casting,
		EGameplayTagEventType::NewOrRemoved
	).AddUObject(
		WaitCastTimeChange,
		&UWaitCastTimeChange::CastTagChanged // make my cast time function(from ability system (like the onactiveeffect added function) call on widget component
	);

	// Called when a GE is applied to self (start of cooldown)
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCastTimeChange, &UWaitCastTimeChange::OnActiveEffectAdded);

	return WaitCastTimeChange;
}

void UWaitCastTimeChange::EndTask()
{
	if (!IsValid(ASC)) return;

	ASC->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Player_Casting, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCastTimeChange::CastTagChanged(const FGameplayTag InCastTag, int32 NewCount)
{

	// The cooldown is over, broadcast
	if (NewCount == 0) {
		CastTimeEnd.Broadcast(0.f);
	}

}

void UWaitCastTimeChange::OnActiveEffectAdded_Implementation(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{


	FGameplayTagContainer AssetTags;

	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(FAuraGameplayTags::Get().Player_Casting) || GrantedTags.HasTagExact(FAuraGameplayTags::Get().Player_Casting)) {


		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FAuraGameplayTags::Get().Player_Casting.GetSingleTagContainer());
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

			CastTimeStart.Broadcast(TimeRemaining);

		}

	}


}
