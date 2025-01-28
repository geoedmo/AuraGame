// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BallLightningProjectile.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraElectricitySpline.h"
#include "GameFramework/ProjectileMovementComponent.h"


ABallLightningProjectile::ABallLightningProjectile()
{

}

void ABallLightningProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMovement->InitialSpeed = DefaultSpeed;
	ProjectileMovement->MaxSpeed = DefaultSpeed;
}

void ABallLightningProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

bool ABallLightningProjectile::EnemiesWithinRadius()
{
	if (CurrentlyActiveTargets.Num() > 0)
	{
		return true;
	}

	return false;
}

TArray<AActor*> ABallLightningProjectile::SeekNearByPlayers()
{
	
	// Ignore Targets Already being Attacked.
	TArray<AActor*> ClosestEnemies;
	
	if (!CurrentlyActiveTargets.Num() == 0 && !bSpeedDecreased)
	{
		bSpeedDecreased = true;
		FVector Velocity = ProjectileMovement->Velocity;
		FVector SlowerVelocity = Velocity / SlowDownSpeedFactor;
		ProjectileMovement->Velocity = SlowerVelocity;
	}
	
	TArray<AActor*> IgnoreActors;
	if (CurrentlyActiveTargets.Num() > 0)
	{
		for (auto Target : CurrentlyActiveTargets)
		{
			IgnoreActors.Add(Target);
		}
	}
	
	IgnoreActors.Add(this);
	IgnoreActors.Add(GetOwner());

	TArray<AActor*> OverlappingActors;
	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(this, OverlappingActors, IgnoreActors, SearchRadius, GetActorLocation());


	if (OverlappingActors.Num() > 0)
	{
		UAuraAbilitySystemLibrary::GetClosestTargets(5, OverlappingActors, ClosestEnemies, GetActorLocation());
	}
	
	return ClosestEnemies;
}

void ABallLightningProjectile::SpawnElectrictySplineAttachedToActor(AActor* Enemy)
{
	
	const FVector EnemyLocation = Enemy->GetActorLocation();

	if (CheckIfEnemyHasSplineAttachment(Enemy)) return;
	
	FActorSpawnParameters SpawnParameters;
	
	FVector ActorLocation = GetActorLocation();
	const FVector Forward = GetActorForwardVector();
	const FRotator Rotation = Forward.Rotation();
	
	SpawnParameters.Owner = GetOwner();
	
	AAuraElectricitySpline* NewSpline = GetWorld()->SpawnActor<AAuraElectricitySpline>(
		SplineComponentActorToAdd,
		ActorLocation,
		Rotation,
		SpawnParameters
		);
	
	FAttachmentTransformRules TransformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, true);
	
	NewSpline->OwningBallLightning = this;
	NewSpline->ActorToAttack = Enemy;
	NewSpline->AddSplinePoint(EnemyLocation);

	NewSpline->OnSplineActorRemovalDelegate.AddDynamic(this, &ABallLightningProjectile::RemoveActorFromCurrentlyActiveTargets);

	CurrentlyActiveTargets.Add(Enemy);
}

bool ABallLightningProjectile::CheckIfEnemyHasSplineAttachment(const AActor* Enemy)
{
	for (const auto Target : CurrentlyActiveTargets)
	{
		if (Target == Enemy)
		{
			return true;
		}
	}

	return false;
}

void ABallLightningProjectile::RemoveActorFromCurrentlyActiveTargets(AActor* Target)
{
	CurrentlyActiveTargets.Remove(Target);

	if (CurrentlyActiveTargets.Num() == 0)
	{
		FVector Velocity = ProjectileMovement->Velocity;
		FVector FasterVelocity = Velocity * SlowDownSpeedFactor;
		ProjectileMovement->Velocity = FasterVelocity;
		
		bSpeedDecreased = false;
	}
}
