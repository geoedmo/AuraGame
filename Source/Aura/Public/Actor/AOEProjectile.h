// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageAOE.h"
#include "AuraProjectile.h"
#include "AOEProjectile.generated.h"




class AAuraTrailSpline;
class UCapsuleComponent;
/**
 * 
 */

UCLASS()
class AURA_API AAOEProjectile : public AAuraDamageAOE
{
	GENERATED_BODY()
	
public:
	AAOEProjectile();

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	
private:

	UFUNCTION(BlueprintCallable)
	void AddToTrails(const FVector& CurrentLocation);

	UFUNCTION(BlueprintCallable)
	void RemoveTailSegment();

	UFUNCTION(BlueprintCallable)
	void SpawnTrail();
	
};
