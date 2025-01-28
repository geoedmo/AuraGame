// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraDamageEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraTrailSpline.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Interaction/CombatInterface.h"


AAuraDamageEffectActor::AAuraDamageEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	
	DamageSphere = CreateDefaultSubobject<USphereComponent>("CrystalSphere");
	RootComponent = DamageSphere;
	
	DamageSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DamageSphere->SetGenerateOverlapEvents(true);
	DamageSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DamageSphere->SetCollisionResponseToChannel(ECC_ExcludePlayers, ECR_Ignore);

	DamageCapsule = CreateDefaultSubobject<UCapsuleComponent>("Overlap Capsule");
	DamageCapsule->SetupAttachment(GetRootComponent());
	DamageCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DamageCapsule->SetGenerateOverlapEvents(true);
	DamageCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DamageCapsule->SetCollisionResponseToChannel(ECC_ExcludePlayers, ECR_Ignore);
	
	
}

void AAuraDamageEffectActor::Destroyed()
{
	Super::Destroyed();
	
}

void AAuraDamageEffectActor::BeginPlay()
{
	Super::BeginPlay();

	DamageTickRate = DefaultTickRate;
	
	DamageSphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraDamageEffectActor::OnSphereOverlap);
	DamageSphere->OnComponentEndOverlap.AddDynamic(this, &AAuraDamageEffectActor::OnSphereEndOverlap);
	
	DamageCapsule->OnComponentBeginOverlap.AddDynamic(this, &AAuraDamageEffectActor::OnSphereOverlap);
	DamageCapsule->OnComponentEndOverlap.AddDynamic(this, &AAuraDamageEffectActor::OnSphereEndOverlap);

	OnSpawnedTrailDelegate.AddDynamic(this, &AAuraDamageEffectActor::OnSpawnedTrail);
	
}


void AAuraDamageEffectActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bAOEProjectile || bIsTrail)
	{
		bOverlapped = true;
	}

	const bool bOwner = OtherActor == GetOwner();
	const bool bTimerIsSet = CheckIfActorHasTimerSet(OtherActor);
	const bool bStillOverlapping = ActorIsStillOverlapping(OtherActor);
	
	if (bOwner && !bOverlapped)
	{
		SetLifeSpan(Lifespan);
		bOverlapped = true;
	}
	
	if (!bOwner && bOverlapped && OtherActor->Implements<UCombatInterface>())
	{
		ActorsOverlapping.AddUnique(OtherActor);
		
		if (!bTimerIsSet && !bStillOverlapping)
		{
			StartAOEDamageTimer(OtherActor, AddTimerForActor(OtherActor));
		}
	} 
}

FTimerHandle& AAuraDamageEffectActor::AddTimerForActor(AActor* OtherActor)
{
	const int32 TimerHandleID = TimersToActors.Num();
	const FTimerHandle TimerToStart = GetWorldTimerManager().GenerateHandle(TimerHandleID);

	TimersToActors.Add(OtherActor, TimerToStart);
	return TimersToActors[OtherActor];
}

void AAuraDamageEffectActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const bool bOwner = OtherActor == GetOwner();
	if (OtherActor->Implements<UCombatInterface>() && !bOwner)
	{
		ActorsOverlapping.Remove(OtherActor);
		
		const bool bActorIsOverlapping = ActorIsStillOverlapping(OtherActor);
		const bool bActorTimerStillValid = CheckIfActorHasTimerSet(OtherActor);
		
		if (bActorTimerStillValid && !bActorIsOverlapping)
		{
			RemoveTimerFromActor(OtherActor);
		}
	} 
}

bool AAuraDamageEffectActor::CheckIfActorHasTimerSet(const AActor* ActorToCheck)
{
	for (const auto DamageActor : ArrayOfGroupedActors)
	{
		if (DamageActor)
		{
			if (DamageActor->TimersToActors.Num() > 0)
			{
				if (TimersToActors.Find(ActorToCheck)) // Look for actor in TMap of Actors Mapped to Timers
				{
					return true;
				}
			}
		}
	}
	
	return false;
}

void AAuraDamageEffectActor::StartAOEDamageTimer(AActor* Actor, FTimerHandle& Timer)
{
	if (Actor->Implements<UCombatInterface>())
	{
		AOEDamageDelegate.BindUFunction(this, FName("TickAOEDamage"), Actor);
		GetWorldTimerManager().SetTimer(Timer, AOEDamageDelegate, DamageTickRate,true);
	}
}

void AAuraDamageEffectActor::TickAOEDamage(AActor* ActorToDamage)
{
	if (!IsValid(ActorToDamage)) return;
	if (!ValidAOEDamageTickForActor(ActorToDamage)) return;

	if (HasAuthority())
	{
 		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ActorToDamage))
        {
            AOEDamageParams.TargetAbilitySystemComponent = TargetASC;
            UAuraAbilitySystemLibrary::ApplyDamageEffect(AOEDamageParams);
        }
	}
	
 }

void AAuraDamageEffectActor::TrailOverlapInformation(FTrailInformation InTrailInfo)
{
	if (InTrailInfo.TrailOwner != this) return;
	
	DamageTickRate = InTrailInfo.TrailTrickRate;
	
	OnSphereOverlap(InTrailInfo.OverlappedComponent, InTrailInfo.OtherActor, InTrailInfo.OtherComp, InTrailInfo.OtherBodyIndex, InTrailInfo.bFromSweep, InTrailInfo.SweepResult);
	
}

void AAuraDamageEffectActor::TrailEndOverlapInformation(FTrailInformation InTrailInfo)
{
	if (InTrailInfo.TrailOwner != this) return;

	DamageTickRate = DefaultTickRate;
	
	OnSphereEndOverlap(InTrailInfo.OverlappedComponent, InTrailInfo.OtherActor, InTrailInfo.OtherComp, InTrailInfo.OtherBodyIndex);
	
}

void AAuraDamageEffectActor::AddTrailToGroupedActors(AAuraTrailSpline* InTrail)
{
	//ArrayOfGroupedActors.AddUnique(InTrail);
}

void AAuraDamageEffectActor::OnSpawnedTrail()
{
	Trail->OnSplineSphereCollisionOverlap.AddDynamic(this, &AAuraDamageEffectActor::TrailOverlapInformation);
	Trail->OnSplineSphereCollisionEndOverlap.AddDynamic(this, &AAuraDamageEffectActor::TrailEndOverlapInformation);

	if (Trail->OnSplineSphereCollisionOverlap.IsBound())
	{
		UE_LOG(LogTemp, Display, TEXT("OnSplineSphereCollisionOverlap bound"));
	}
	//AddTrailToGroupedActors(Trail);
}

FDamageEffectParams& AAuraDamageEffectActor::GetDamageParams()
{
	return AOEDamageParams;
}

void AAuraDamageEffectActor::SetDamageEffectParams(FDamageEffectParams DamageParams)
{
	AOEDamageParams = DamageParams;
}

void AAuraDamageEffectActor::SetArrayOfGroupedActors(const TArray<AAuraDamageEffectActor*>& InArray)
{
	ArrayOfGroupedActors = InArray;
}

AAuraTrailSpline* AAuraDamageEffectActor::GetTrail()
{
	return Trail;
}

bool AAuraDamageEffectActor::ActorIsStillOverlapping(AActor* ActorToCheck)
{
	for (const auto DamageActor : ArrayOfGroupedActors)
	{
		if (DamageActor)
		{
			if (DamageActor->ActorsOverlapping.Num() > 0)
			{
				if (DamageActor->ActorsOverlapping.Find(ActorToCheck) != INDEX_NONE) return true;
			}
		}
	}
	return false;
}

void AAuraDamageEffectActor::RemoveTimerFromActor(const AActor* Actor)
{
	for (const auto DamageActor : ArrayOfGroupedActors)
	{
		if (DamageActor && DamageActor->TimersToActors.Num() > 0)
		{
			if (FTimerHandle* TimerToClear = DamageActor->TimersToActors.Find(Actor))
			{
				DamageActor->GetWorldTimerManager().ClearTimer(*TimerToClear);
				DamageActor->TimersToActors.Remove(Actor);
				return;
			}
		}
	}
}

bool AAuraDamageEffectActor::ValidAOEDamageTickForActor(AActor* ActorToDamage)
{
	if (ActorToDamage->Implements<UCombatInterface>())
	{
		const bool bStillOverlapping = ActorIsStillOverlapping(ActorToDamage);
		if (ICombatInterface::Execute_IsDead(ActorToDamage) || !bStillOverlapping)
		{
			RemoveTimerFromActor(ActorToDamage);
			return false;
		}
	}
	return true;
}
