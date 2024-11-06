// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVMLoadMenu.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FButtonEnable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTestButtonEnable, bool, bButtonEnabled);

class UMVVM_LoadSlot;
/**
 * 
 */
UCLASS()
class AURA_API UMVVMLoadMenu : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	void InitializeLoadSlots();
	
	UPROPERTY(BlueprintAssignable)
	FButtonEnable ButtonEnable;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 Slot, const FString& EnteredName);
	
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Slot);

	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 Slot);
	
	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();

	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed();
	
	void LoadData();

	/* Field Notifies */
	void SetLoadMenuName(FString InMenuName);

	FString GetLoadMenuName() const { return LoadMenuName; }
	
private:

	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> SelectedSlot;
    	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = true));
	FString LoadMenuName;
};
