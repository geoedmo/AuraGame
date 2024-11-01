// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AuraGameModeBase.h"

#include "Game/LoadMenuSaveObject.h"
#include "Kismet/GameplayStatics.h"
#include "ModelView/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);

	//cast to LoadMenuSaveObject ( our created Save Object)
	ULoadMenuSaveObject* LoadMenuSaveObject = Cast<ULoadMenuSaveObject>(SaveGameObject);
	LoadMenuSaveObject->PlayerName = LoadSlot->GetPlayerName();
	LoadMenuSaveObject->SaveSlotStatus = Taken;
	
	UGameplayStatics::SaveGameToSlot(LoadMenuSaveObject, LoadSlot->GetLoadSlotName(), SlotIndex);

}

ULoadMenuSaveObject* AAuraGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{

	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	ULoadMenuSaveObject* LoadMenuSaveGame = Cast<ULoadMenuSaveObject>(SaveGameObject);
	return LoadMenuSaveGame;

	
}
