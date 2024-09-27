// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PointCollectionActor.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"


/*
 * Optional Macro if wanted, submitted by student
 *
#define CREATE_AND_SETUP_PT(Pt_X) \
PT_X = CreateDefaultSubobject<USceneComponent>(TEXT(#PT_X)); \
ImmutablePts.Add(PT_X); \
PT_X->SetupAttachment(GetRootComponent());
CREATE_AND_SETUP_PT(PT_1);
CREATE_AND_SETUP_PT(PT_2);
CREATE_AND_SETUP_PT(PT_3);
CREATE_AND_SETUP_PT(PT_4);
CREATE_AND_SETUP_PT(PT_5);
CREATE_AND_SETUP_PT(PT_6);
CREATE_AND_SETUP_PT(PT_7);
CREATE_AND_SETUP_PT(PT_8);
CREATE_AND_SETUP_PT(PT_9);
CREATE_AND_SETUP_PT(PT_10);
 */
 
APointCollectionActor::APointCollectionActor()
{
	PrimaryActorTick.bCanEverTick = false;

	PT_0 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_0"));
	ImmutablePoints.Add(PT_0);
	SetRootComponent(PT_0);

	PT_1 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_1"));
	ImmutablePoints.Add(PT_1);
	PT_1->SetupAttachment(GetRootComponent());
	
	PT_2 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_2"));
	ImmutablePoints.Add(PT_2);
	PT_2->SetupAttachment(GetRootComponent());
	
	PT_3 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_3"));
	ImmutablePoints.Add(PT_3);
	PT_3->SetupAttachment(GetRootComponent());
	
	PT_4 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_4"));
	ImmutablePoints.Add(PT_4);
	PT_4->SetupAttachment(GetRootComponent());
	
	PT_5 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_5"));
	ImmutablePoints.Add(PT_5);
	PT_5->SetupAttachment(GetRootComponent());
	
	PT_6 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_6"));
	ImmutablePoints.Add(PT_6);
	PT_6->SetupAttachment(GetRootComponent());
	
	PT_7 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_7"));
	ImmutablePoints.Add(PT_7);
	PT_7->SetupAttachment(GetRootComponent());
	
	PT_8 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_8"));
	ImmutablePoints.Add(PT_8);
	PT_8->SetupAttachment(GetRootComponent());
	
	PT_9 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_9"));
	ImmutablePoints.Add(PT_9);
	PT_9->SetupAttachment(GetRootComponent());
	
	PT_10 = CreateDefaultSubobject<USceneComponent>(TEXT("PT_10"));
	ImmutablePoints.Add(PT_10);
	PT_10->SetupAttachment(GetRootComponent());
	
}

TArray<USceneComponent*> APointCollectionActor::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride)
{
	checkf(ImmutablePoints.Num() >= NumPoints, TEXT("Attempted to access Immutable points out of bounds."))

	TArray<USceneComponent*> ArrayCopy;

	for (USceneComponent* Pt : ImmutablePoints)
	{
		if (ArrayCopy.Num() >= NumPoints) return ArrayCopy;

		if (Pt != PT_0)
		{
			FVector ToPoint = Pt->GetComponentLocation() - PT_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Pt->SetWorldLocation(PT_0->GetComponentLocation() + ToPoint);
		}

		const FVector RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		const FVector LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);

		FHitResult HitResult;
		TArray<AActor*> IgnoreActors;
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(this, IgnoreActors, TArray<AActor*>(), 1500.f, GetActorLocation());

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), QueryParams);

		const FVector AdjustedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		ArrayCopy.Add(Pt);
		
	}

	return ArrayCopy;

}


void APointCollectionActor::BeginPlay()
{
	Super::BeginPlay();
	
}




