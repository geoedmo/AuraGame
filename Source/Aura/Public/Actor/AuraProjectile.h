// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraAbilityTypes.h"
#include "GameplayTagContainer.h"
#include "AuraProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class USoundBase;
class UAudioComponent;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	AAuraProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartOrigin = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn="true"));
	FDamageEffectParams DamageEffectParams;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ProjectileDamageType;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* HomingTargetSceneComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<AActor> SourceAvatarActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<USoundBase> LoopingSound;
	
	UPROPERTY(EditAnywhere, Category = "Projectile")
	TObjectPtr<UNiagaraSystem> ImpactSystem;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TObjectPtr<USoundBase> ImpactSound;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void Destroyed() override;


	UFUNCTION(BlueprintCallable)
	virtual void OnHit();

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	float GetDistanceFromStartOrigin();

	void StopLoopingSound();

	bool IsValidOverlap(AActor* OtherActor);
	
	bool bHit = false;

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;

private:









};
