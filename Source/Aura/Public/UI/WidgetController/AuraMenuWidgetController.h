// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraMenuWidgetController.generated.h"





class UAuraAttributeInfo;
struct FGameplayTag;

UCLASS(BlueprintType, Blueprintable)
class AURA_API UAuraMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;

	virtual void BroadcastInitialValues() override;

	UFUNCTION(BlueprintCallable)
	bool CheckForAttributePoints();


	UFUNCTION(BlueprintCallable)
	void SetActiveInMenu(bool InMenuStatus);

	UFUNCTION(BlueprintCallable)
	void UpgradeAttributePoints(const FGameplayTag& AttibuteTag);

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeDataSignature AttributeDataDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnAttributePointsChangedSignature;





protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAuraAttributeInfo> AttributeInfo;

private:



};
