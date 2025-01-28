// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AOEProjectile.h"
#include "NiagaraComponent.h"
#include "Actor/AuraTrailSpline.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AAOEProjectile::AAOEProjectile()
{
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
	ProjectileMovement->InitialSpeed = 50.f;
	ProjectileMovement->MaxSpeed = 50.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	DamageSphere->Deactivate();
	
}

void AAOEProjectile::BeginPlay()
{
	Super::BeginPlay();

	const FAttachmentTransformRules FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	NiagaraComponent->AttachToComponent(DamageCapsule, FAttachmentTransformRules);

	NiagaraComponent->Activate();
	SetLifeSpan(Lifespan);

	if (bLeavesTrails)
	{
		SpawnTrail();
	}

}

void AAOEProjectile::Destroyed()
{
	if (Trail)
	{
		Trail->Destroy();
	}
	
	Super::Destroyed();
}

void AAOEProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
}

void AAOEProjectile::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	
}

void AAOEProjectile::AddToTrails(const FVector& CurrentLocation)
{
	if (Trail)
	{
		Trail->AddToSpline(CurrentLocation);
		Trail->DropCollisionSphere(CurrentLocation);
	}
}

void AAOEProjectile::RemoveTailSegment()
{
	if (Trail)
	{
		Trail->RemoveTailSegment();
	}
}

void AAOEProjectile::SpawnTrail()
{
	FActorSpawnParameters SpawnParameters;
	
	FVector ActorLocation = GetActorLocation();
	const FVector Forward = GetActorForwardVector();
	const FRotator Rotation = Forward.Rotation();
	ActorLocation.Z = 0.f; // need to get a hit result for any higher spawn locations 

	SpawnParameters.Owner = GetOwner();
	
	Trail = GetWorld()->SpawnActor<AAuraTrailSpline>(
		TrailClass,
		ActorLocation,
		Rotation,
		SpawnParameters
		);

	Trail->SetOwner(GetOwner());
	Trail->TrailOwner = this;
	
	OnSpawnedTrailDelegate.Broadcast();
	// uses this actors damage params for the trail.
	
}