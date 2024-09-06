// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraBeamSpell.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"

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

void UAuraBeamSpell::PerformFirstTraceFromWeapon()
{

	FHitResult HitResult;

	if (StoredOwnerCharacter->Implements<UCombatInterface>())
	{
		USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(StoredOwnerCharacter);
		const FVector SocketLocation = Weapon->GetSocketLocation("TipSocket");
		TArray<AActor*> ActorsToIgnore;

		UKismetSystemLibrary::SphereTraceSingle(StoredOwnerCharacter,
			SocketLocation,
			MouseHitLocation,
			5.f,
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResult,
			true,
			FColor::Red,
			FColor::Green,
			10.f
		);

	}

	if (HitResult.bBlockingHit) {

		MouseHitLocation = HitResult.Location;
		MouseHitActor = HitResult.GetActor();

	}


}
