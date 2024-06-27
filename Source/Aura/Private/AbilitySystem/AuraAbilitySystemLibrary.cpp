// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

UAuraOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();

			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();

			UAttributeSet* AS = PS->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);

		}
	}

	return nullptr;
}

UAuraMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();

			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();

			UAttributeSet* AS = PS->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

			return AuraHUD->GetMenuWidgetController(WidgetControllerParams);

		}
	}

	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeClassDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{

	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return;

	UCharacterClassInfo* CharacterClassInfo = AuraGameMode->CharacterClassInfo;
	FCharacterClassDefaultInfo CharacterDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributeContextHandle = ASC->MakeEffectContext();
	PrimaryAttributeContextHandle.AddSourceObject(ASC->GetAvatarActor());
	FGameplayEffectSpecHandle PrimaryAttributesEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterDefaultInfo.PrimaryAttributes, Level, PrimaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesEffectSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributeContextHandle = ASC->MakeEffectContext();
	SecondaryAttributeContextHandle.AddSourceObject(ASC->GetAvatarActor());
	FGameplayEffectSpecHandle SecondaryAttributesEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesEffectSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributeContextHandle = ASC->MakeEffectContext();
	VitalAttributeContextHandle.AddSourceObject(ASC->GetAvatarActor());
	FGameplayEffectSpecHandle VitalAttributesEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesEffectSpecHandle.Data.Get());

}
