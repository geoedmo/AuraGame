// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint/Checkpoint.h"

#include "Aura/Aura.h"
#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/AuraPlayerInterface.h"
#include "Kismet/GameplayStatics.h"

ACheckpoint::ACheckpoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
	PrimaryActorTick.bCanEverTick = false;
	

	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointMesh");
	CheckpointMesh->SetupAttachment(GetRootComponent());
	CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckpointMesh->SetCollisionResponseToAllChannels(ECR_Block);

	MoveToComponent = CreateDefaultSubobject<USceneComponent>("MoveToComponent");
	MoveToComponent->SetupAttachment(GetRootComponent());
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(CheckpointMesh);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	
}

void ACheckpoint::HighlightActor_Implementation()
{
	if (!bReached)
	{
		CheckpointMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_TAN);
        CheckpointMesh->SetRenderCustomDepth(true);
	}

}

void ACheckpoint::UnhighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(false);
}

void ACheckpoint::SetMoveToLocation_Implementation(FVector& OutLocation)
{
	OutLocation = MoveToComponent->GetComponentLocation();
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	if (bBindOverlapCallback)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnSphereOverlap);
	}
		
}

void ACheckpoint::HandleGlowEffects()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(CheckpointMesh->GetMaterial(0), this);
	CheckpointMesh->SetMaterial(0, DynamicMaterialInstance);
	CheckPointReached(DynamicMaterialInstance);
}

void ACheckpoint::LoadActor_Implementation()
{
	if (bReached)
	{
		HandleGlowEffects();
	}
}

void ACheckpoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->Implements<UAuraPlayerInterface>())
	{
			bReached = true;

			if (AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
			{
				const UWorld* World = GetWorld();
				FString MapName = World->GetMapName();
				MapName.RemoveFromStart(World->StreamingLevelsPrefix);
			
				AuraGM->SaveWorldState(GetWorld(), MapName);
			}
			
		IAuraPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
		HandleGlowEffects();
	}
	
}


