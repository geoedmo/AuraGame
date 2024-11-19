// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

/**
 * 
 */

class AAuraEffectActor;

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AAuraEffectActor> LootClass;

	UPROPERTY(EditAnywhere)
	int32 MaxItemsToSpawn = 1;

	UPROPERTY(EditAnywhere)
	float ChanceToSpawn = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOverrideItemLevel = false;
};

UCLASS()
class AURA_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetSpawnedLootItems();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLootItem> LootItems;
	
};
