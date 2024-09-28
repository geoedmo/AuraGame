// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1) {
		
		return FString::Printf(TEXT(
			
			//Title
			"<Title>Arcane Shards</>\n"
			
			//Level
			"<Small>Level:</> <Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost:</> <ManaCost>%.1f</>\n"
			//ManaCost
			"<Small>CoolDown:</> <Cooldown>%.1f</><Small> secs</>\n\n"
			
			//Description
			"<Default>This spell casts a magic circle that summons a column of crystal shards, dealing</>"

			//Damage Amount
			
			"<Damage> %d</><Default> "

			//Description
			"arcane damage to all enemies in range of the shard.</> \n"),

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
			"<Title>Arcane Shards</>\n"
			
			//Level
			"<Small>Level:</> <Level>%d</>\n"
			
			//ManaCost
			"<Small>ManaCost:</> <ManaCost>%.1f</>\n"

			//Cooldown
			"<Small>CoolDown:</> <Cooldown>%.1f</> <Small> secs</>\n\n"

			//Number of Firebolts
			"<Default>This Spell casts a magic circle that summons %d columns of crystal shards"

			//Description
			" at the ground location dealing "

			//Damage Amount
			"</><Damage>%d</><Default>"
			
			//Description
			" to any enemy caught within the range of the crystals.</> \n"),

			//Variables
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxShards),
			ScaledDamage);

	}

}

FString UArcaneShards::GetNextLevelDescription(int32 Level)
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
		"<Default>This Spell casts a magic circle that summons %d columns of crystal shards"

		//Description
		" at the ground location dealing "

		//Damage Amount
		"</><Damage>%d</><Default>"

		//Description
		" to any enemy caught within the range of the crystals.</> \n"),

		//Variables
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxShards),
		ScaledDamage);

}
