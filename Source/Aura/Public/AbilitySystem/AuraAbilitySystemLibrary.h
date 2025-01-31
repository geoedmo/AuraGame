// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "Data/LootTiers.h"
#include "Game/LoadMenuSaveObject.h"

#include "AuraAbilitySystemLibrary.generated.h"

/**
 * 
 */
class USpellMenuWidgetController;
class UOverlayWidgetController;
struct FDamageEffectParams;
struct FWidgetControllerParams;
class ULoadScreenSaveGame;

UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Ability System Startup Functions
	 */
	
	UFUNCTION(Blueprintcallable, Category = "AuraAbilitySystemLibrary|Class Default Information")
	static void InitializeClassDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(Blueprintcallable, Category = "AuraAbilitySystemLibrary|Class Default Information")
	static void InitializeClassDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ULoadMenuSaveObject* SaveGame);
	
	UFUNCTION(Blueprintcallable, Category = "AuraAbilitySystemLibrary|Class Default Information")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Class Default Information")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Class Default Information")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	/**
	 * Widget Controller and UI
	 */
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAuraOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAuraMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static int32 GetXPForCharacterClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 Level);

	/**
	 * Effect Context Getters
	 */
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
	static bool IsAOEDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
	static bool IsSuccessfulKnockback(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Damage Effects")
    static FVector GetKnockback (const FGameplayEffectContextHandle& EffectContextHandle);
    
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Damage Effects")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
	static float GetDeathImpulseMagnitude(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
	static float RadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
	static float RadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);

	
	/**
	 * Effect Context Setters
	 */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
    static void SetIsBlockedHit(UPARAM(ref)FGameplayEffectContextHandle EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
	static void SetIsAOEDamage(UPARAM(ref)FGameplayEffectContextHandle EffectContextHandle, bool bInIsBlockedHit);
	
    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
    static void SetIsCriticalHit(UPARAM(ref)FGameplayEffectContextHandle EffectContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Damage Effects")
	static void SetIsSuccessfulDebuff(UPARAM(ref)FGameplayEffectContextHandle EffectContextHandle, bool bInIsSuccessfulDebuff, float InDebuffDamage, float InDebuffDuration, float InDebuffFrequency, const FGameplayTag& InDamageType);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Damage Effects")
    static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle EffectContextHandle, const FVector& InDeathImpulse);

    UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Damage Effects")
    static void SetDeathImpulseMagnitude(UPARAM(ref) FGameplayEffectContextHandle EffectContextHandle, float InDeathImpulseMagnitude);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
	static void SetIsSuccessfulKnockback(UPARAM(ref)FGameplayEffectContextHandle EffectContextHandle, bool bInIsKnockback);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Damage Effects")
	static void SetKnockback(UPARAM(ref) FGameplayEffectContextHandle EffectContextHandle, const FVector& InKnockback);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
	static void SetIsRadialDamage(UPARAM(ref)FGameplayEffectContextHandle EffectContextHandle, bool bInIsRadialDamage);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Damage Effects")
	static void SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle EffectContextHandle, const FVector& InRadialDamageOrigin);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Damage Effects")
	static void SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle EffectContextHandle, float InRadialDamageOuterRadius);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Damage Effects")
	static void SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle EffectContextHandle, float InRadialDamageInnerRadius);

	
	/**
	 * Damage Parameters
	 */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	/**
	 * Ability Helper Functions
	 */
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& FowardVector, const FVector& Axis, float Spread, int32 NumRotators);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static TArray<FVector> EvenlySpacedVectors(const FVector& FowardVector, const FVector& Axis, float Spread, int32 NumVectors);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Gameplay Effects")
    static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& InActors, TArray<AActor*>& OutClosestTargets, const FVector& Origin);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

	/*
	 * Damage Effect Params
	 */

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void SetIsRadialDamageEffectParams(UPARAM(ref) FDamageEffectParams& ExplosionEffectParams, bool bInIsRadialDamage,  float RadialDamageInnerRadius, float RadialDamageOuterRadius, FVector InRadialDamageOrigin);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void SetTargetDamageEffectParamsASC(UPARAM(ref) FDamageEffectParams& ExplosionEffectParams, UAbilitySystemComponent* InTargetASC);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void SetKnockbackParams(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float InKnockbackChance = 100.f, float Magnitude = 0.f);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void SetDeathImpulseDirection(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude = 0.f);

	/*
	 * Loot Tiers
	 */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|LootTiers", meta = (DefaultToSelf = "WorldContextObject"))
	static ULootTiers* GetLootTierInfo (const UObject* WorldContextObject);
};
