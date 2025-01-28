// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraDamageAOE.h"
#include "NiagaraComponent.h"


AAuraDamageAOE::AAuraDamageAOE()
{
	PrimaryActorTick.bCanEverTick = false;
	
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraSystem");
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->bAutoActivate = false;
	
}

void AAuraDamageAOE::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraDamageAOE::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (OtherActor == GetOwner() && !bActivated && !bAOEProjectile)
	{
		bActivated = true;
		NiagaraComponent->Activate();
	}
	
}

void AAuraDamageAOE::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}



