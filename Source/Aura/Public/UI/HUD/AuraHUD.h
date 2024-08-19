// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

/**
 * 
 */
struct FWidgetControllerParams;
class UAuraOverlayWidgetController;
class UAuraMenuWidgetController;
class UAuraUserWidget;
class UAbilitySystemComponent;
class UAttributeSet;
class USpellMenuWidgetController;


UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:

	/** Controller Getters **/
	UAuraOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAuraMenuWidgetController* GetMenuWidgetController(const FWidgetControllerParams& WCParams);
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:


private:

	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	/** Controllers **/

	//Attribute Menu Controller
	UPROPERTY()
	TObjectPtr<UAuraMenuWidgetController> AuraMenuWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraMenuWidgetController> AuraMenuWidgetControllerClass;


	//Spell Menu Controller
	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;

	//HUD Overlay Controller
	UPROPERTY()
	TObjectPtr<UAuraOverlayWidgetController> AuraOverlayWidgetController;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraOverlayWidgetController> AuraOverlayWidgetControllerClass;
	
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;




};
