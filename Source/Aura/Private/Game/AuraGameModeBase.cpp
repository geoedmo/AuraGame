// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AuraGameModeBase.h"

#include "CookOnTheSide/CookOnTheFlyServer.h"
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
	LoadMenuSaveObject->MapName = LoadSlot->GetMapName();
	LoadMenuSaveObject->SaveSlotStatus = Taken;
	
	UGameplayStatics::SaveGameToSlot(LoadMenuSaveObject, LoadSlot->GetLoadSlotName(), SlotIndex);

}

void AAuraGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

void AAuraGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->GetLoadSlotName();
	const int32 SlotIndex = Slot->GetSlotIndex();
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, GameMaps.FindChecked(Slot->GetMapName()));
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

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GameMaps.Add(DefaultMapName, DefaultMap);
}
