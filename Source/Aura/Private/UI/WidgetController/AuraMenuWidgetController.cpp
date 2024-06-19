// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraAttributeInfo.h"
#include "AuraGameplayTags.h"

void UAuraMenuWidgetController::BindCallbacksToDependencies()
{

}

void UAuraMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);


	for (auto& Pair : AS->TagsToAttributes) {
	FAuraAttributeData Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
	FGameplayAttribute AttributeToWorkOn = Pair.Value();
	// Need the Numerical Value of this
	Info.AttributeValue = AttributeToWorkOn.GetNumericValue(AS);

	AttributeDataDelegate.Broadcast(Info);

	}

}
