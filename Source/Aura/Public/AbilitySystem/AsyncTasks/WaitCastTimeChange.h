// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "WaitCastTimeChange.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCasttimeChangeSignature, float, TimeRemaining);

/**
 * 
 */

class UAbilitySystemComponent;
struct FGameplayEffectSpec;

UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class AURA_API UWaitCastTimeChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:

	UWaitCastTimeChange();


	UPROPERTY(BlueprintAssignable)
	FCasttimeChangeSignature CastTimeStart;

	UPROPERTY(BlueprintAssignable)
	FCasttimeChangeSignature CastTimeEnd;
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInteralUseOnly = "true"))
	static UWaitCastTimeChange* WaitForCastCompletion(UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintCallable)
	void EndTask();


protected:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	void CastTagChanged(const FGameplayTag InCastTag, int32 NewCount);

	UFUNCTION(Client, Reliable)
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle);

};
