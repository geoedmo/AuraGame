// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "AuraProjectile.h"
#include "AuraDamageEffectActor.generated.h"

struct FTrailInformation;
class UCapsuleComponent;
class USphereComponent;
class UNiagaraComponent;
class AAuraTrailSpline;
class UAuraAbilitySystemComponent;
class AAuraCrystal;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnedTrailSignature);

UCLASS()
class AURA_API AAuraDamageEffectActor : public AActor
{
	GENERATED_BODY()
	
public:
	AAuraDamageEffectActor();
	virtual void BeginPlay() override;

	FOnSpawnedTrailSignature OnSpawnedTrailDelegate;

protected:
	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsTrail = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLeavesTrails = false;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraTrailSpline> TrailClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<AAuraTrailSpline> Trail;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AOE DAMAGE")
	TObjectPtr<USphereComponent> DamageSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> DamageCapsule;

	UPROPERTY(EditDefaultsOnly)
	float Lifespan = 8.f;

	UPROPERTY(EditDefaultsOnly)
	bool bAOEProjectile = false;
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category = "AOE")
	bool bOverlapped = false;
	
	UFUNCTION()
	void OnSpawnedTrail();

private:

	FTimerDelegate AOEDamageDelegate;

	UPROPERTY(VisibleAnywhere, Category = "AOE DAMAGE")
	FDamageEffectParams AOEDamageParams;

	UPROPERTY(EditDefaultsOnly, Category = "AOE DAMAGE")
	float DefaultTickRate = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, Category = "AOE DAMAGE")
	float DamageTickRate = 0.5f;

	UPROPERTY(EditInstanceOnly, Category = "AOE DAMAGE")
	TArray<AAuraDamageEffectActor*> ArrayOfGroupedActors;
	
	UPROPERTY(VisibleAnywhere, Category = AOE)
	TArray<AActor*> ActorsOverlapping;

	UPROPERTY(VisibleAnywhere, Category = AOE)
	TMap<AActor*, FTimerHandle> TimersToActors;
	
	bool CheckIfActorHasTimerSet(const AActor* ActorToCheck);
	bool ActorIsStillOverlapping(AActor* ActorToCheck);
	void RemoveTimerFromActor(const AActor* Actor);
	FTimerHandle& AddTimerForActor(AActor* OtherActor);
	void StartAOEDamageTimer(AActor* Actor, FTimerHandle& Timer);
	bool ValidAOEDamageTickForActor(AActor* ActorToDamage);

	UFUNCTION()
	void TickAOEDamage(AActor* ActorToDamage);

	UFUNCTION()
	void TrailOverlapInformation(FTrailInformation InTrailInfo);

	UFUNCTION()
	void TrailEndOverlapInformation(FTrailInformation InTrailInfo);

	void AddTrailToGroupedActors(AAuraTrailSpline* InTrail);

;
public:
	UFUNCTION(BlueprintCallable)
	FDamageEffectParams& GetDamageParams();

	UFUNCTION(BlueprintCallable)
	void SetDamageEffectParams(FDamageEffectParams DamageParams);

	UFUNCTION(BlueprintCallable)
	void SetArrayOfGroupedActors(const TArray<AAuraDamageEffectActor*>& InArray);

	UFUNCTION(BlueprintCallable)
	AAuraTrailSpline* GetTrail();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetLifeSpanOfProjectile() { return Lifespan; }
};
