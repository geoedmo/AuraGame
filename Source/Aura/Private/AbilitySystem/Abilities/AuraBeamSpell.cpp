// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraBeamSpell.h"
#include "GameFramework/Character.h"

void UAuraBeamSpell::StoreMouseInfoData(const FHitResult& MouseHit)
{
	if (MouseHit.bBlockingHit) {
	MouseHitLocation = MouseHit.Location;
	MouseHitActor = MouseHit.GetActor();
	}
	else {
	
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}

}

void UAuraBeamSpell::StoreOwnerVariables()
{


	if (CurrentActorInfo) {

		StoredOwnerController = CurrentActorInfo->PlayerController.Get();

		StoredOwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}


}
