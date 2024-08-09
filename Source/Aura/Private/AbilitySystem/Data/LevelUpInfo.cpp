// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP)
{

	int32 Level = 1;
	bool bSearching = true;
	while (bSearching) {

		//LevelUpInformation[1] = Level 1 Information
		//LevelUpInformation[2] = Level 2 Information
		if (LevelUpInformation.Num() - 1 <= 1) return Level;

			if (XP >= LevelUpInformation[Level].LevelUpRequirement) {
				++Level;
			}
			else
			{

				bSearching = false;

			}

	}

	return Level;
}