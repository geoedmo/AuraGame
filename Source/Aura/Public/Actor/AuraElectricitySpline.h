// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraElectricitySpline.generated.h"

class ABallLightningProjectile;
class UNiagaraComponent;
class USplineComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSplineActorRemovalSignature, AActor*, ActorToDelete);

UCLASS()
class AURA_API AAuraElectricitySpline : public AActor
{
	GENERATED_BODY()
	
public:	

	AAuraElectricitySpline();

	FOnSplineActorRemovalSignature OnSplineActorRemovalDelegate;
	
	UFUNCTION()
	void UpdateLocations();

	UFUNCTION(BlueprintCallable)
	bool ShouldStillAttackEnemy(const FVector& EnemyLocation);

	UPROPERTY(EditDefaultsOnly, Category = Spline)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly, Category = Spline)
	TObjectPtr<UNiagaraComponent> NiagaraSystem;

	void AddSplinePoint(const FVector& Point);

	UPROPERTY(VisibleDefaultsOnly, Category = Spline)
	TObjectPtr<ABallLightningProjectile> OwningBallLightning;

	UPROPERTY(VisibleDefaultsOnly, Category = Spline)
	TObjectPtr<AActor> ActorToAttack;

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Spline)
	float AttackUpdateRate = 0.06f;

private:

	UPROPERTY(EditAnywhere, Category = "Spline")
	float AcceptanceRadius = 25.f;

	UPROPERTY(VisibleAnywhere, Category = Spline)
	FRotator SecondPointRotation = FRotator();
	
	UPROPERTY(VisibleAnywhere, Category = Spline)
	FVector SecondPointUnitVector = FVector::ZeroVector;

	FTimerHandle AttackEnemy;




};
