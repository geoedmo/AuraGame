// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraCrystalLash.h"

#include "AuraGameplayTags.h"
#include "Actor/AuraDamageAOE.h"
#include "Aura/Aura.h"

TArray<FVector> UAuraCrystalLash::ReturnLineOfAuraCrystals()
{
	TArray<FVector> SpawnPoints;
	
	float DistanceBetweenActors = DistanceToTravel / NumPoints;
	FVector ActorLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	
	FRotator ActorRotation = (MouseHitLocation - ActorLocation).Rotation();
	ActorRotation.Pitch = 0.f;
	ActorRotation.Roll = 0.f;
	float RotationYaw = -ActorRotation.Yaw;
	FVector ActorNewForward = ActorRotation.Vector();

	UE_LOG(LogTemp, Warning, TEXT("Angle to Rotate: %f"), RotationYaw);

	FVector SpawnLocation;
	SpawnLocation = (ActorNewForward * DistanceBetweenActors) + ActorLocation;
	
	for (int32 i = 0; i < NumPoints; i++)
	{
		// Run a trace at each spawn location to get ground impact.
		FHitResult HitResult;
		FVector RaisedLocation = SpawnLocation;
		RaisedLocation.Z = 100.f;
		FVector LoweredLocation = SpawnLocation;
		LoweredLocation.Z = -100.f;
		
		TArray<AActor*> IgnoreActors;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByChannel( HitResult, RaisedLocation, LoweredLocation, ECC_ExcludePlayers, QueryParams);
		
		const FVector AdjustedLocation = FVector(SpawnLocation.X, SpawnLocation.Y, HitResult.ImpactPoint.Z);
		SpawnPoints.Add(AdjustedLocation);
		
		SpawnLocation += ActorNewForward * DistanceBetweenActors;
		
	}
	return SpawnPoints;
}

TArray<AAuraDamageEffectActor*> UAuraCrystalLash::SpawnCrystals()
{
	TArray<AAuraDamageEffectActor*> Crystals;
	TArray<FVector> Points = ReturnLineOfAuraCrystals();

	int32 PointsLength = Points.Num();

	for (int32 i = 0; i < PointsLength; i++)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Points[i]);

		AAuraDamageEffectActor* Crystal = GetWorld()->SpawnActorDeferred<AAuraDamageEffectActor>(
			AuraCrystalClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		Crystal->GetDamageParams() = MakeDamageEffectParamsFromClassDefaults();
		Crystal->SetOwner(GetAvatarActorFromActorInfo());
		Crystals.Add(Crystal);
		
		Crystal->FinishSpawning(SpawnTransform);
		
	}

	for (auto Crystal : Crystals)
	{;
		Crystal->SetArrayOfGroupedActors(Crystals);
	}
	
	return Crystals;
}

