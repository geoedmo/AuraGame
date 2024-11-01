// Fill out your copyright notice in the Description page of Project Settings.


#include "ModelView/MVVM_LoadSlot.h"
#include "Game/LoadMenuSaveObject.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	const int32 WidgetSwitcherIndex = SlotStatus.GetValue();
	SetWidgetSwitcherIndex.Broadcast(WidgetSwitcherIndex);
}

void UMVVM_LoadSlot::SetLoadSlotName(FString InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}

void UMVVM_LoadSlot::SetPlayerName(FString InPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);

}

void UMVVM_LoadSlot::SetSlotIndex(int32 InSlotIndex)
{
	UE_MVVM_SET_PROPERTY_VALUE(SlotIndex, InSlotIndex);
}
