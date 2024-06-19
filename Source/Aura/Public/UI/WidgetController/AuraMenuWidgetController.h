// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraMenuWidgetController.generated.h"

struct FAuraAttributeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeDataSignature, const FAuraAttributeData&, Info);

/**
 * 
 */

class UAuraAttributeInfo;

UCLASS(BlueprintType, Blueprintable)
class AURA_API UAuraMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;


	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeDataSignature AttributeDataDelegate;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAuraAttributeInfo> AttributeInfo;

private:

};
