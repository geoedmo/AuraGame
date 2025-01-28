// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 * 
 * Singleton containing natie Gameplay Tags
 */


struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();


	/**	
	* Gameplay Tag Variables
	**/						
	
	/** Primary Attributes **/
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	/** Secondary Attributes **/
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	/* Meta Attributes */
	FGameplayTag Attributes_Meta_IncomingXP;

	/* Resistances */
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;

	/** Input Gameplay Tags **/
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	/** Abilities **/
	FGameplayTag Abilities_None;
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	// Lightning
	FGameplayTag Abilities_Lightning_Electrocute;
	FGameplayTag Abilities_Lightning_LightningOrb;
	// Arcane
	FGameplayTag Abilities_Arcane_ArcaneShards;
	FGameplayTag Abilities_Arcane_ArcaneShardLash;
	// Fire
	FGameplayTag Abilities_Fire_FirenadoStorm;
	FGameplayTag Abilities_Fire_FireBlast;
	FGameplayTag Abilities_Fire_FireBolt;
	
	FGameplayTag Abilities_HitReact;

	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Equipped;

	FGameplayTag Abilities_Status_Offensive;
	FGameplayTag Abilities_Status_Passive;
	FGameplayTag Abilities_Status_None;
	
	/** Passive Abilities **/
	FGameplayTag Abilities_Passive_HaloOfProtection;
	FGameplayTag Abilities_Passive_LifeSiphon;
	FGameplayTag Abilities_Passive_ManaSiphon;

	/** Cooldowns **/
	FGameplayTag Cooldowns_Fire_FirenadoStorm;
	FGameplayTag Cooldowns_Fire_FireBolt;
	FGameplayTag Cooldowns_Fire_FireBlast;
	FGameplayTag Cooldowns_Lightning_Electrocute;
	FGameplayTag Cooldowns_Lightning_LightningOrb;
	FGameplayTag Cooldowns_Arcane_ArcaneShards;
	FGameplayTag Cooldowns_Arcane_ArcaneShardLash;

	/** Combat Socket Tags **/
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;

	/** Montage Tags **/
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;


	/* Combat Related */
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
	
	/*Player Tags*/
	FGameplayTag Player_Block_CursorTrace;
	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_InputHeld;
	/* Cast Tags*/
	FGameplayTag Player_Casting;


	/* Debuffs */
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Physical;
	FGameplayTag Debuff_Stun;

	/* Debuff Properties */
	FGameplayTag Debuff_KnockbackChance;
	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequency;


	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;

	/* Gameplay Effects */
	FGameplayTag Effects_HitReact;

	/* Gameplay Cues*/
	FGameplayTag GameplayCue_FireBlast;

private:
	static FAuraGameplayTags GameplayTags;
};
