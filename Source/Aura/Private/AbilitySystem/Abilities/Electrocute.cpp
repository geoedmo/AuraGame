// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
const float ManaCost = FMath::Abs(GetManaCost(Level));
const float Cooldown = GetCooldown(Level);
if (Level == 1) {
		
	return FString::Printf(TEXT(
			
		//Title
		"<Title>Electrocute</>\n"
			
		//Level
		"<Small>Level:</> <Level>%d</>\n"
		//ManaCost
		"<Small>ManaCost:</> <ManaCost>%.1f</>\n"
		//ManaCost
		"<Small>CoolDown:</> <Cooldown>%.1f</><Small> secs</>\n\n"
			
		//Description
		"<Default>This spell casts a beam into the target electrocuting it for</>"

		//Damage Amount
			
		"<Damage> %d</><Default> "

		//Description
		" lightning damage with a 100 percent chance to stun at the end of the cast.</> \n"),

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
		"<Default>This spell casts a beam into the highlighted target, and closest %d targets "

		//Description
		"dealing "

		//Damage Amount
		"</><Damage>%d</><Default>"
			
		//Description
		" lightning damage to all with a 100 percent chance to stun at the end of the cast.</> \n"),

		//Variables
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxShockEnemies),
		ScaledDamage);

}
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
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
		"<Default>This spell casts a beam into the highlighted target, and closest %d targets "

		//Description
		"dealing "

		//Damage Amount
		"</><Damage>%d</><Default>"

		//Description
		" lightning damage to all with a 100 percent chance to stun at the end of the cast.</> \n"),

		//Variables
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, MaxShockEnemies),
		ScaledDamage);
}
