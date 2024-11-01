// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraLoadMenuHUD.h"

#include "Blueprint/UserWidget.h"
#include "ModelView/MVVMLoadMenu.h"

#include "UI/Widget/LoadMenuWidget.h"

void AAuraLoadMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	
	LoadScreenViewModel = NewObject<UMVVMLoadMenu>(this, LoadScreenViewModelClass);
	LoadScreenViewModel->InitializeLoadSlots();
	
	LoadMenuWidget = CreateWidget<ULoadMenuWidget>(GetWorld(), LoadMenuWidgetClass);
	LoadMenuWidget->AddToViewport();
	LoadMenuWidget->BlueprintInitializeWidget();

	LoadScreenViewModel->LoadData();
}
