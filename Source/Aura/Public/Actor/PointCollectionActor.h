// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointCollectionActor.generated.h"

UCLASS()
class AURA_API APointCollectionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APointCollectionActor();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* PT_0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* PT_1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* PT_2;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* PT_3;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* PT_4;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* PT_5;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* PT_6;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* PT_7;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* PT_8;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* PT_9;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* PT_10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<USceneComponent*> ImmutablePoints;

	UFUNCTION(BlueprintPure)
	TArray<USceneComponent*> GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride = 0.f);
	
};
