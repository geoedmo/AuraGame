// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraOverlayWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AuraGameplayTags.h"

void UAuraOverlayWidgetController::BroadcastInitialValues()
{

	OnHealthChanged.Broadcast(GetAuraAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());

}

void UAuraOverlayWidgetController::BindCallbacksToDependencies()
{

	GetAuraASC()->AbilityEquippedDelegate.AddUObject(this, &UAuraOverlayWidgetController::OnAbilityEquipped);

	// Added a couple Local variables for doing some work below.
	AuraPlayerState = GetAuraPS();
	AuraPlayerState->OnXPChangedDelegate.AddUObject(this, &UAuraOverlayWidgetController::ReceiveXPInformation);

	AuraAttributeSet = GetAuraAS();
	UAuraAbilitySystemComponent* AuraASC = GetAuraASC();

	AuraPlayerState->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel) {

			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);

		}
	);

	/* Old way of binding callbacks before we learned of the Lambda Expressions
	* 
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()).AddUObject(this, &UAuraOverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UAuraOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()).AddUObject(this, &UAuraOverlayWidgetController::ManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UAuraOverlayWidgetController::MaxManaChanged);
	*
	*  ^^^ OLD WAY
	*/ 

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) {
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) {
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) {
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) {
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	if (AuraASC)
	{
		// Bind to the Delegate telling us the Startup Abilities are given.

		if (AuraASC->bStartupAbilitiesGiven) {

			BroadcastAbilityInfo();

		}
		else
		{

			AuraASC->AbilitiesGivenDelegate.AddUObject(this, &UAuraOverlayWidgetController::BroadcastAbilityInfo);

		}

		AuraASC->EffectAssetTags.AddLambda(

			[this](const FGameplayTagContainer& AssetTags)
			{

				for (const FGameplayTag& Tag : AssetTags)
				{
					//For example, say that Tag - Message.HealthPotion
					//"Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False

					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));

					if (Tag.MatchesTag(MessageTag)) {
						FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}


				}

			}

		);

		// Still have the ASC Check here, so Going to bind to XP delegate:


	}


}
void UAuraOverlayWidgetController::ReceiveXPInformation(int32 NewXP)
{
	AuraPlayerState = GetAuraPS();
	ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo, Please fillout AuraPlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0) {

		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;
		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement; // How much XP Total is in the Current Level's XP Bar
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement; // How much XP currently have for CURRENT level

		const float XPBarPercentForThisLevel = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement); // Percent of the Current level's bar filled

		OnXPPercentChangedDelegate.Broadcast(XPBarPercentForThisLevel);
	}


}

void UAuraOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot) const
{

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	FAuraAbilityInfo LastSlotInfo;

	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PrevSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;

	// Broadcast Empty info if Prev Slot is a Valid Slot
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);

	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

}



/* OLD CALL BACKS NO LONGER USED
void UAuraOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UAuraOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UAuraOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UAuraOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
*/