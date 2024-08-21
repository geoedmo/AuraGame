// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();

	OnSpellPointsChangedSignature.Broadcast(GetAuraPS()->GetPlayerSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{

		GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda(

		[this](int32 SpellPoints) {

			OnSpellPointsChangedSignature.Broadcast(SpellPoints);

		});

	GetAuraASC()->AbilityStatusChangedDelegate.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatus) {

			if (AbilityInfo) {

				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = AbilityStatus;
				AbilityInfoDelegate.Broadcast(Info);

			}

		}
	);

}
