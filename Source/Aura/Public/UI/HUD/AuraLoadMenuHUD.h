// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraLoadMenuHUD.generated.h"

class UMVVMLoadMenu;
class ULoadMenuWidget;
/**
 * 
 */
UCLASS()
class AURA_API AAuraLoadMenuHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadMenuWidgetClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULoadMenuWidget> LoadMenuWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVMLoadMenu> LoadScreenViewModelClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVMLoadMenu> LoadScreenViewModel;
	
private:

	virtual void BeginPlay() override;
	
};
