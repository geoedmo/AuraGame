// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AuraAttributeInfo.h"

FAuraAttributeData UAuraAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FAuraAttributeData& Info : AttributeInformation)
	{
		if (Info.AttributeTag == AttributeTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for Attribute Tag [%s] on AttributeInfo[%s]."), *AttributeTag.ToString(),*GetNameSafe(this));
	}

	return FAuraAttributeData();

}