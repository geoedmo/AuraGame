// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TravelArrowSystem.h"

#include "NiagaraComponent.h"

ATravelArrowSystem::ATravelArrowSystem()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	
	TravelArrowSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DirectionArrow"));
	TravelArrowSystem->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ATravelArrowSystem::BeginPlay()
{
	Super::BeginPlay();
	
}


