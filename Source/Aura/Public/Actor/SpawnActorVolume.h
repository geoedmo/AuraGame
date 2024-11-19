// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "SpawnActorVolume.generated.h"

class AAuraEnemySpawnPoint;
class UBoxComponent;

UCLASS()
class AURA_API ASpawnActorVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	ASpawnActorVolume();

	/* ISaveInterface */
	void LoadActor_Implementation() override;
	/* /ISaveInterface */

	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bReached = false;
	
protected:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> Box;
	
	UPROPERTY(EditAnywhere)
	TArray<AAuraEnemySpawnPoint*> EnemySpawnPoints;
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
