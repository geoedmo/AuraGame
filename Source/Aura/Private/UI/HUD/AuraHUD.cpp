// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "UI/WidgetController/AuraMenuWidgetController.h"
#include "UI/WidgetController/AuraOverlayWidgetController.h"


UAuraOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AuraOverlayWidgetController == nullptr) {
		AuraOverlayWidgetController = NewObject<UAuraOverlayWidgetController>(this, AuraOverlayWidgetControllerClass);
		AuraOverlayWidgetController->SetWidgetControllerParams(WCParams);
		AuraOverlayWidgetController->BindCallbacksToDependencies();

		return AuraOverlayWidgetController;
	}


	return AuraOverlayWidgetController;
}

UAuraMenuWidgetController* AAuraHUD::GetMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AuraMenuWidgetController == nullptr) {
		AuraMenuWidgetController = NewObject<UAuraMenuWidgetController>(this, AuraMenuWidgetControllerClass);
		AuraMenuWidgetController->SetWidgetControllerParams(WCParams);
		AuraMenuWidgetController->BindCallbacksToDependencies();

		return AuraMenuWidgetController;
	}


	return AuraMenuWidgetController;
}

USpellMenuWidgetController* AAuraHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SpellMenuWidgetController == nullptr)
	{
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(WCParams);
		SpellMenuWidgetController->BindCallbacksToDependencies();
	}

	return SpellMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{

	checkf(OverlayWidgetClass, TEXT("Overlay Widgetclass Uninitialized, fill out BP_AuraHUD"));
	checkf(AuraOverlayWidgetControllerClass, TEXT("Overlay Widget Controller not Initialized, fill out BP_AuraHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

	UAuraOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);

	WidgetController->BroadcastInitialValues();
	


	if (Widget) {
		Widget->AddToViewport();
	}

}

