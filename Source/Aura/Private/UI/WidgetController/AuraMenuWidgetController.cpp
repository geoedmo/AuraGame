// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraAttributeInfo.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AttributeSet.h"


void UAuraMenuWidgetController::BindCallbacksToDependencies()
{

	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
	
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair, AS](const FOnAttributeChangeData& Data) {

				FAuraAttributeData Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
				Info.AttributeValue = Pair.Value().GetNumericValue(AS);
				AttributeDataDelegate.Broadcast(Info);
			}

		);
	}

	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);

	AuraPlayerState->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 AttributePoints) {
			OnAttributePointsChangedSignature.Broadcast(AttributePoints);
		});

	AuraPlayerState->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 SpellPoints) {
			OnSpellPointsChangedSignature.Broadcast(SpellPoints);
		});

}

void UAuraMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);


	for (auto& Pair : AS->TagsToAttributes) {

		FAuraAttributeData Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		AttributeDataDelegate.Broadcast(Info);

	}

	/** Broadcast Initial Values for SpellPoints and Attribute Points **/
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	OnAttributePointsChangedSignature.Broadcast(AuraPlayerState->GetPlayerAttributePoints());
	OnSpellPointsChangedSignature.Broadcast(AuraPlayerState->GetPlayerSpellPoints());
}

bool UAuraMenuWidgetController::CheckForAttributePoints()
{
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);

	bool bPlayerHasAttributePoints = AuraPlayerState->GetPlayerAttributePoints() > 0;

	if (bPlayerHasAttributePoints)
		return true;
	else {
		return false;
	}

}

void UAuraMenuWidgetController::SetActiveInMenu(bool InMenuStatus)
{
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);

	AuraPlayerState->SetInMenus(InMenuStatus);
}

void UAuraMenuWidgetController::UpgradeAttributePoints(const FGameplayTag& AttibuteTag)
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);

	AuraASC->UpgradeAttributes(AttibuteTag);
}


