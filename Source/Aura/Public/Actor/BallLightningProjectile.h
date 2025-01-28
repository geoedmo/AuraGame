// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "BallLightningProjectile.generated.h"

class AAuraElectricitySpline;
class AAuraElectricitySpline;
class UNiagaraComponent;
class UNiagaraSystem;
class USplineComponent;
/**
 *  This class will handle the special functions of the ball lightning class, specifically with respect to
 *  player seeking behavior timers and utilizing Splines to create effects that look like lightning is
 *  going to the near-by players.
 */
UCLASS()
class AURA_API ABallLightningProjectile : public AAuraProjectile
{
	GENERATED_BODY()

public:

	ABallLightningProjectile();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraElectricitySpline> SplineComponentActorToAdd;

protected:

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:

	bool bSpeedDecreased = false;

	UPROPERTY(EditDefaultsOnly)
	float DefaultSpeed = 500.f;
	
	UPROPERTY(EditDefaultsOnly)
	float SlowDownSpeedFactor = 3.f;

	UFUNCTION(BlueprintCallable)
	bool EnemiesWithinRadius();
	
	UPROPERTY(VisibleDefaultsOnly)
	int32 InstanceCount = 0;
	
	UPROPERTY(VisibleAnywhere)
	TArray<AAuraElectricitySpline*> SplineNiagaraComponents;

	UPROPERTY(VisibleAnywhere)
	TArray<UNiagaraSystem*> NiagaraSystems;

	UPROPERTY(VisibleAnywhere)
	TArray<USplineComponent*> SplineComponents;
	
	UPROPERTY(EditDefaultsOnly)
	float SearchRadius = 300.f;

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> SeekNearByPlayers();

	UFUNCTION(BlueprintCallable)
	void SpawnElectrictySplineAttachedToActor(AActor* Enemy);

	bool CheckIfEnemyHasSplineAttachment(const AActor* Enemy);
	
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> CurrentlyActiveTargets;

	UFUNCTION()
	void RemoveActorFromCurrentlyActiveTargets(AActor* Target);
public:

	FORCEINLINE float GetSearchRadius() const { return SearchRadius; }
};
