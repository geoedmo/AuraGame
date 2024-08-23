// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
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

		CurrentSpellPoints = SpellPoints;
		bool bEnabledSpendPoints = false;
		bool bEnableEquip = false;
		ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnabledSpendPoints, bEnableEquip);
		FString Description;
		FString NextLevelDescription;
		GetAuraASC()->GetDescriptionsbyAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
		SpellGlobeSelectedDelegate.Broadcast(bEnabledSpendPoints, bEnableEquip, Description, NextLevelDescription);
	});


	GetAuraASC()->AbilityStatusChangedDelegate.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatus, int32 Level) {

			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag)) {
				SelectedAbility.Status = AbilityStatus;
				bool bEnabledSpendPoints = false;
				bool bEnableEquip = false;
				ShouldEnableButtons(AbilityStatus, CurrentSpellPoints, bEnabledSpendPoints, bEnableEquip);
				FString Description;
				FString NextLevelDescription;
				GetAuraASC()->GetDescriptionsbyAbilityTag(AbilityTag, Description, NextLevelDescription);
				SpellGlobeSelectedDelegate.Broadcast(bEnabledSpendPoints, bEnableEquip, Description, NextLevelDescription);
			}

			if (AbilityInfo) {

				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = AbilityStatus;
				AbilityInfoDelegate.Broadcast(Info);

			}

		}
	);

}

void USpellMenuWidgetController::SpendSpellPoint()
{
	GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
}

void USpellMenuWidgetController::GlobeDeselect()
{
	SelectedAbility.Ability = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FAuraGameplayTags::Get().Abilities_Status_Locked;

	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}


void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	FGameplayTag AbilityStatus;
	const int32 SpellPoints = GetAuraPS()->GetPlayerSpellPoints();
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr; // Spec Valid but Locked

	if (!bTagValid || bTagNone || !bSpecValid) {

		AbilityStatus = GameplayTags.Abilities_Status_Locked;

	}
	else
	{
		AbilityStatus = GetAuraASC()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;

	bool bEnabledSpendPoints = false;
	bool bEnableEquip = false;

	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnabledSpendPoints, bEnableEquip);
	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionsbyAbilityTag(AbilityTag, Description, NextLevelDescription);
	SpellGlobeSelectedDelegate.Broadcast(bEnabledSpendPoints, bEnableEquip, Description, NextLevelDescription);

}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	bShouldEnableEquipButton = false;
	bShouldEnableSpellPointsButton = false;
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped)) {

		bShouldEnableEquipButton = true;
		if (SpellPoints > 0) {
			bShouldEnableSpellPointsButton = true;
		}

	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0) {

			bShouldEnableSpellPointsButton = true;

		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked)) 
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0) {

			bShouldEnableSpellPointsButton = true;

		}
	}
}

bool USpellMenuWidgetController::CheckForSpellPoints()
{
	AuraPlayerState = GetAuraPS();

	bool bPlayerHasSpellPoints = AuraPlayerState->GetPlayerSpellPoints() > 0;

	if (bPlayerHasSpellPoints)
		return true;
	else {
		return false;
	}

}


