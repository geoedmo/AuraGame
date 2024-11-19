// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SpawnActorVolume.h"

#include "Actor/AuraEnemySpawnPoint.h"
#include "Components/BoxComponent.h"
#include "Interaction/AuraPlayerInterface.h"

ASpawnActorVolume::ASpawnActorVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(GetRootComponent());

	
}

void ASpawnActorVolume::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnComponentBeginOverlap.AddDynamic(this, &ASpawnActorVolume::OnBoxOverlap);
	
}

void ASpawnActorVolume::LoadActor_Implementation()
{
	if (bReached)
	{
		Destroy();
	}
}

void ASpawnActorVolume::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (IsValid(OtherActor) && OtherActor->Implements<UAuraPlayerInterface>())
	{
		bReached = true;
		
		for (auto Enemy : EnemySpawnPoints)
		{
			
			Enemy->SpawnEnemy();
			
		}
	}
	
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}



