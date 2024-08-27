// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1) {
		
		return FString::Printf(TEXT(
			
			//Title
			"<Title>Fire Bolt</>\n"
			
			//Level
			"<Small>Level:</> <Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost:</> <ManaCost>%.1f</>\n"
			//ManaCost
			"<Small>CoolDown:</> <Cooldown>%.1f</><Small> secs</>\n\n"
			
			//Description
			"<Default>This Spell Launches a firebolt at the target, dealing</>"

			//Damage Amount
			
			"<Damage> %d</><Default> "

			//Description
			" fire damage with a chance to burn.</> \n"),

			//Variables
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>Fire Bolt</>\n"
			
			//Level
			"<Small>Level:</> <Level>%d</>\n"
			
			//ManaCost
			"<Small>ManaCost:</> <ManaCost>%.1f</>\n"

			//Cooldown
			"<Small>CoolDown:</> <Cooldown>%.1f</> <Small> secs</>\n\n"

			//Number of Firebolts
			"<Default>This Spell Launches %d bolts of fire"

			//Description
			" at the target dealing "

			//Damage Amount
			"</><Damage>%d</><Default>"
			
			//Description
			" fire damage with a chance to burn.</> \n"),

			//Variables
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			ScaledDamage);

	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);

	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		//Title
		"<Title>Next Level</>\n"

		//Level
		"<Small>Level:</> <Level>%d</>\n"

		//ManaCost
		"<Small>ManaCost:</> <ManaCost>%.1f</>\n"

		//Cooldown
		"<Small>CoolDown:</> <Cooldown>%.1f</><Small> secs</>\n\n"

		//Number of Firebolts
		"<Default>This Spell Launches %d bolts of fire"

		//Description
		" at the target dealing "

		//Damage Amount
		"</><Damage>%d</><Default>"

		//Description
		" fire damage with a chance to burn.</> \n"),

		//Variables
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, NumProjectiles),
		ScaledDamage);

}
