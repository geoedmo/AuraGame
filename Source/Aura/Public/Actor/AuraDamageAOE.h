// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageEffectActor.h"
#include "GameFramework/Actor.h"
#include "AuraDamageAOE.generated.h"

UCLASS()
class AURA_API AAuraDamageAOE : public AAuraDamageEffectActor
{
	GENERATED_BODY()
	
public:
	AAuraDamageAOE();

protected:
	
	/* Variables */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Niagara Component")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual void BeginPlay() override;

private:
	bool bActivated = false;
};
