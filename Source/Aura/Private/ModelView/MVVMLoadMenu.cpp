// Fill out your copyright notice in the Description page of Project Settings.


#include "ModelView/MVVMLoadMenu.h"

#include "Game/AuraGameModeBase.h"
#include "Game/LoadMenuSaveObject.h"
#include "Kismet/GameplayStatics.h"
#include "ModelView/MVVM_LoadSlot.h"

void UMVVMLoadMenu::InitializeLoadSlots()
{
	SetLoadMenuName(FString ("MainLoadMenu"));
	
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlots.Add(0, LoadSlot_0);
	
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlots.Add(1, LoadSlot_1);
	
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlots.Add(2, LoadSlot_2);
	
}

UMVVM_LoadSlot* UMVVMLoadMenu::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVMLoadMenu::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));

	LoadSlots[Slot]->SetPlayerName(EnteredName);
	LoadSlots[Slot]->SlotStatus = Taken;
	
	AuraGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializeSlot();
}

void UMVVMLoadMenu::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVMLoadMenu::SelectSlotButtonPressed(int32 Slot)
{
	// This delegate allows the Play and Delete buttons to be Enabled when a slot is selected, it is assigned in the LoadMenu widget.
	ButtonEnable.Broadcast();
	
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		if (Slot == LoadSlot.Key)
		{
			LoadSlot.Value->SlotSelectButtonEnabled.Broadcast(false);
		} else
		{
			LoadSlot.Value->SlotSelectButtonEnabled.Broadcast(true);
		}
	}
}

void UMVVMLoadMenu::LoadData()
{
	// Loop Over TMap and Retrieve Saved Data.
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(AuraGameMode)) return;
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		ULoadMenuSaveObject* SaveObject = AuraGameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);

		const FString PlayerName = SaveObject->PlayerName;
		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;

		LoadSlot.Value->SlotStatus = SaveSlotStatus;
		LoadSlot.Value->SetPlayerName(PlayerName);
		LoadSlot.Value->InitializeSlot();
		
	}
}

void UMVVMLoadMenu::SetLoadMenuName(FString InMenuName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadMenuName, InMenuName);
}
