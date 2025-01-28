// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TravelArrowSystem.generated.h"

class UNiagaraComponent;
UCLASS()
class AURA_API ATravelArrowSystem : public AActor
{
	GENERATED_BODY()
	
public:	

	ATravelArrowSystem();
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraComponent> TravelArrowSystem;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FVector Location;

	virtual void BeginPlay() override;

};
