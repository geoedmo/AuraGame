// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraTrailSpline.h"
#include "NiagaraComponent.h"
#include "Aura/Aura.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Interaction/CombatInterface.h"

AAuraTrailSpline::AAuraTrailSpline()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SplineComponent->SetupAttachment(GetRootComponent());
	
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("Niagara System");
	NiagaraComponent->SetupAttachment(SplineComponent);

}


void AAuraTrailSpline::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraTrailSpline::OnSplineOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const bool bOwner = OtherActor == GetOwner();
	if (!OtherActor->Implements<UCombatInterface>() || bOwner) return;
	
	ActorsOverlapping.Add(OtherActor);
	
	FTrailInformation TrailInfo;
	TrailInfo.OtherActor = OtherActor;
	TrailInfo.OverlappedComponent = OverlappedComponent;
	TrailInfo.OtherComp = OtherComp;
	TrailInfo.OtherBodyIndex = OtherBodyIndex;
	TrailInfo.bFromSweep = bFromSweep;
	TrailInfo.SweepResult = SweepResult;
	TrailInfo.TrailOwner = TrailOwner;
	TrailInfo.ReferenceToThisTrail = this;
	
	OnSplineSphereCollisionOverlap.Broadcast(TrailInfo);

	bIsOnSpline = true;
}

void AAuraTrailSpline::OnSplineEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const bool bOwner = OtherActor == GetOwner();
	if (!OtherActor->Implements<UCombatInterface>() || bOwner) return;
	

	if (!bOwner)
	{
		ActorsOverlapping.RemoveSingle(OtherActor);
	}
	
	const bool bIsStillOverlapping = ActorIsStillOverlapping(OtherActor);

	if (!bIsStillOverlapping)
	{
		FTrailInformation TrailInfo;
        TrailInfo.OtherActor = OtherActor;
        TrailInfo.OverlappedComponent = OverlappedComponent;
        TrailInfo.OtherComp = OtherComp;
        TrailInfo.OtherBodyIndex = OtherBodyIndex;
        TrailInfo.TrailOwner = TrailOwner;
        TrailInfo.ReferenceToThisTrail = this;
		
        OnSplineSphereCollisionEndOverlap.Broadcast(TrailInfo);
	}
}

bool AAuraTrailSpline::ActorIsStillOverlapping(AActor* OtherActor)
{
	for (const auto Actor : ActorsOverlapping)
	{
		if (Actor)
		{
			if (ActorsOverlapping.Num() > 0)
			{
				if (ActorsOverlapping.Find(OtherActor) != INDEX_NONE) return true;
			}
		}
	}
	return false;
}

void AAuraTrailSpline::AddToSpline(const FVector& NewSplinePoint)
{
	SplineComponent->AddSplinePointAtIndex(NewSplinePoint,SplineComponent->GetNumberOfSplinePoints(), ESplineCoordinateSpace::World, true);
	
}

TArray<USphereComponent*> AAuraTrailSpline::GetCollisionSphereArray()
{
	return CollisionSpheres;
}

USplineComponent* AAuraTrailSpline::GetSpline()
{

	return SplineComponent;
}

void AAuraTrailSpline::RemoveTailSegment()
{

	if (SplineComponent->GetNumberOfSplinePoints() > 0)
	{
		SplineComponent->RemoveSplinePoint(0);
	}
	
	if (CollisionSpheres.Num() > 0)
	{
		CollisionSpheres[0]->DestroyComponent();
		CollisionSpheres.RemoveAt(0, EAllowShrinking::Yes);
	}
	
}

void AAuraTrailSpline::DropCollisionSphere(const FVector& Location)
{
	USphereComponent* NewDamageSphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass());
	
	CollisionSpheres.Add(NewDamageSphere);
	const int32 SphereIndex = CollisionSpheres.Num() - 1;
	CollisionSpheres[SphereIndex]->SetWorldLocation(Location);
	CollisionSpheres[SphereIndex]->SetSphereRadius(50.f);
	CollisionSpheres[SphereIndex]->RegisterComponent();
	CollisionSpheres[SphereIndex]->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionSpheres[SphereIndex]->SetGenerateOverlapEvents(true);
	CollisionSpheres[SphereIndex]->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionSpheres[SphereIndex]->SetCollisionResponseToChannel(ECC_ExcludePlayers, ECR_Ignore);
	CollisionSpheres[SphereIndex]->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	CollisionSpheres[SphereIndex]->OnComponentBeginOverlap.AddDynamic(this, &AAuraTrailSpline::OnSplineOverlap);
	CollisionSpheres[SphereIndex]->OnComponentEndOverlap.AddDynamic(this, &AAuraTrailSpline::OnSplineEndOverlap);

	
	UE_LOG(LogTemp, Warning, TEXT("Sphere Location %s"), *CollisionSpheres[SphereIndex]->GetComponentLocation().ToString());
	
}


