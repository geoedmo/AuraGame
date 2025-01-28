// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraTrailSpline.generated.h"

class USphereComponent;
class AAuraTrailSpline;
class AAuraDamageEffectActor;
class AAOEProjectile;
class USplineMeshComponent;
class UNiagaraComponent;
class USplineComponent;

USTRUCT(BlueprintType)
struct FTrailInformation
{
	
	GENERATED_BODY()

	FTrailInformation(){};

	UPROPERTY()
	TObjectPtr<AAuraTrailSpline> ReferenceToThisTrail = nullptr;

	UPROPERTY()
	TObjectPtr<AAuraDamageEffectActor> TrailOwner = nullptr;

	UPROPERTY()
	float TrailTrickRate = 0.25f;
	
	UPROPERTY()
	UPrimitiveComponent* OverlappedComponent = nullptr;

	UPROPERTY()
	AActor* OtherActor = nullptr;

	UPROPERTY()
	UPrimitiveComponent* OtherComp = nullptr;

	UPROPERTY()
	int32 OtherBodyIndex = 0;

	UPROPERTY()
	bool bFromSweep = false;
	
	FHitResult SweepResult = FHitResult();
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSplineSphereCollisionOverlap, FTrailInformation, TrailPackage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSplineSphereCollisionEndOverlap, FTrailInformation, TrailPackage);

UCLASS()
class AURA_API AAuraTrailSpline : public AActor
{
	GENERATED_BODY()
	
public:
	AAuraTrailSpline();

	FOnSplineSphereCollisionOverlap OnSplineSphereCollisionOverlap;
	FOnSplineSphereCollisionEndOverlap OnSplineSphereCollisionEndOverlap;
	
	UPROPERTY(EditDefaultsOnly, Category = Spline)
	TObjectPtr<USplineComponent> SplineComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = Spline)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(VisibleAnywhere, Category = Spline)
	TArray<USphereComponent*> CollisionSpheres;

	UFUNCTION(BlueprintCallable, Category = Spline)
	void AddToSpline(const FVector& NewSplinePoint);

	UFUNCTION(BlueprintCallable, Category = Spline)
	USplineComponent* GetSpline();
	
	UFUNCTION(BlueprintCallable, Category = Spline)
	TArray<USphereComponent*> GetCollisionSphereArray();
	
	UFUNCTION(BlueprintCallable, Category = Spline)
	void RemoveTailSegment();
	
	UFUNCTION(BlueprintCallable)
	void DropCollisionSphere(const FVector& Location);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AAuraDamageEffectActor> TrailOwner;

	UPROPERTY(VisibleAnywhere, Category = AOE)
	TArray<AActor*> ActorsOverlapping;
	
protected:
	bool bIsOnSpline = false;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSplineOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSplineEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> FirstOverlappedSphere = nullptr;

	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> NextOverlappedSphere = nullptr;

	bool ActorIsStillOverlapping(AActor* OtherActor);

};
