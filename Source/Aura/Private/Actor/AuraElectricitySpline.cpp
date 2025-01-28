// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraElectricitySpline.h"
#include "NiagaraComponent.h"
#include "Actor/BallLightningProjectile.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"


AAuraElectricitySpline::AAuraElectricitySpline()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	RootComponent = Spline;
	
	NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	NiagaraSystem->SetupAttachment(Spline);
	NiagaraSystem->bAutoActivate = true;
	
}

void AAuraElectricitySpline::BeginPlay()
{
	Super::BeginPlay();
	
	// Set Timer To Attack Enemy which will Call UpdateLocations
	GetWorldTimerManager().SetTimer(
		AttackEnemy,
		this,
		&AAuraElectricitySpline::UpdateLocations,
		AttackUpdateRate,
		true
	);
	
}
void AAuraElectricitySpline::UpdateLocations()
{
	if (ShouldStillAttackEnemy(ActorToAttack->GetActorLocation()))
	{
		SecondPointRotation = UKismetMathLibrary::FindLookAtRotation(OwningBallLightning->GetActorLocation(),ActorToAttack->GetActorLocation());
		SecondPointRotation.Pitch = 0.f;
		SecondPointRotation.Roll = 0.f;
		
		SecondPointUnitVector = SecondPointRotation.Vector();
		FVector SecondPointLocationToMove = OwningBallLightning->GetActorLocation() + (SecondPointUnitVector * 75.f);
		
		Spline->SetLocationAtSplinePoint(0, OwningBallLightning->GetActorLocation(), ESplineCoordinateSpace::World);
		Spline->SetLocationAtSplinePoint(1, SecondPointLocationToMove, ESplineCoordinateSpace::World);
		
		int32 SplineIndex = Spline->GetNumberOfSplinePoints() - 1;
		Spline->SetLocationAtSplinePoint(SplineIndex, ActorToAttack->GetActorLocation(), ESplineCoordinateSpace::World, true);
		Spline->UpdateSpline();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(AttackEnemy);
		OnSplineActorRemovalDelegate.Broadcast(ActorToAttack);
		Destroy();
	}
}

bool AAuraElectricitySpline::ShouldStillAttackEnemy(const FVector& EnemyLocation)
{
	float DistanceToBallLightning = (EnemyLocation - OwningBallLightning->GetActorLocation()).Length();

	UE_LOG(LogTemp, Warning, TEXT("Distance To Orb: %f"), DistanceToBallLightning);
	if (DistanceToBallLightning > OwningBallLightning->GetSearchRadius() + AcceptanceRadius)
	{
		return false;
	}

	return true;
}


void AAuraElectricitySpline::AddSplinePoint(const FVector& Point)
{
	Spline->AddSplineWorldPoint(Point);
	Spline->UpdateSpline(); 
}


