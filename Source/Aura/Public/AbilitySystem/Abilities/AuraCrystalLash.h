// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageAbility.h"
#include "AuraCrystalLash.generated.h"

class AAuraDamageAOE;
UCLASS()
class AURA_API UAuraCrystalLash : public UAuraDamageAbility
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Arcane Crystal Lash")
	TObjectPtr<AAuraDamageAOE> AuraCrystal;

	UPROPERTY(EditDefaultsOnly, Category = "Arcane Crystal Lash")
	TSubclassOf<AAuraDamageAOE> AuraCrystalClass;
	
	FDamageEffectParams CrystalDamageParams;
	
	UFUNCTION(BlueprintCallable, Category = "Arcane Crystal Lash")
	TArray<FVector> ReturnLineOfAuraCrystals();

	UFUNCTION(BlueprintCallable)
	TArray<AAuraDamageEffectActor*> SpawnCrystals();
		
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	FVector MouseHitLocation;
	
	UPROPERTY(EditDefaultsOnly, Category = "Arcane Crystal Lash")
	int32 NumPoints;

	UPROPERTY(EditDefaultsOnly, Category = "Arcane Crystal Lash")
	float DistanceToTravel = 1200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arcane Crystal Lash")
	float DistanceBetweenCrystals = 150.f;
	
	
};
