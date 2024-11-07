// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadMenuSaveObject.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

UCLASS()
class AURA_API ULoadMenuSaveObject : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString SlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	FString MapName = FString("Default Map Name");

	UPROPERTY()
	FString PlayerName = FString("Default Name");

	UPROPERTY()
	FName PlayerStartTag; // Cant have spaces, Fstrings can
	
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;

	/* Player */

	UPROPERTY()
	int32 PlayerLevel = 0;

	UPROPERTY()
	int32 XP = 0;

	UPROPERTY()
	int32 SpellPoints = 0;

	UPROPERTY()
	int32 AttributePoints = 0;

	/* Attributes */
	
	UPROPERTY()
	float Strength = 0.f;

	UPROPERTY()
	float Intellect = 0.f;

	UPROPERTY()
	float Vigor = 0.f;

	UPROPERTY()
	float Resilience = 0.f;
	
};
