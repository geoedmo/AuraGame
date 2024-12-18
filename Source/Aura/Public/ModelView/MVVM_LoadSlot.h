// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadMenuSaveObject.h"
#include "MVVM_LoadSlot.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectSlotButtonEnable, bool, bEnable);

/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;
	
	UPROPERTY(BlueprintAssignable)
	FSelectSlotButtonEnable SlotSelectButtonEnabled;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	UPROPERTY()
	FString MapAssetName;

	UPROPERTY()
	FName PlayerStartTag;
	
	void InitializeSlot();
	
	/*Field Notifies Setters/Getters*/
	void SetLoadSlotName(FString InLoadSlotName);
	void SetPlayerName(FString InPlayerName);
	void SetSlotIndex(int32 InSlotIndex);
	void SetMapName(FString InMapName);
	void SetPlayerLevel(int32 InPlayerLevel);
	
	
	FString GetLoadSlotName() const { return LoadSlotName; };
	FString GetPlayerName() const { return PlayerName; };
	int32 GetSlotIndex() const { return SlotIndex; };
	FString GetMapName() const { return MapName; };
	int32 GetPlayerLevel() const { return PlayerLevel; };
private:
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true));
	FString PlayerName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true));
	FString LoadSlotName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true));
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true));
	int32 PlayerLevel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true));
	int32 SlotIndex;
};
