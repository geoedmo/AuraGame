// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

/**
 * 
 */

class UCharacterClassInfo;
class UAbilityInfo;

UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Character Class Info")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditAnywhere, Category = "Character Class Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;
};
