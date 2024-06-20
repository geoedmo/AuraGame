// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"
#include "InputAction.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{


	for (const FAuraInputAction& AbilityInputAction : AbilityInputActions) {

		//For example, say that Tag - Message.HealthPotion
		//"Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
		

		if (AbilityInputAction.InputAction && AbilityInputAction.InputTag == InputTag) {
		
			return AbilityInputAction.InputAction;
		}
	}


	if (bLogNotFound) {

		UE_LOG(LogTemp, Error, TEXT("Cant find AbilityInputAction for inputTag[%s], or InputConfig[%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
