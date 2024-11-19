// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

UENUM(BlueprintType)
enum EEffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum EEffectRemovalPolicy
{
	RemoveOnEndOverlap,
	DoNotRemove
};

class UAbilitySystemComponent;
class UGameplayEffect;
class USoundBase;

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AAuraEffectActor();
	
	virtual void Tick(float DeltaTime) override;
	
protected:

	virtual void BeginPlay() override;

	/** Variables **/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	TObjectPtr<USoundBase> HitGroundImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	TObjectPtr<USoundBase> SpawnSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
	TObjectPtr<USoundBase> ConsumedSound;
	
	UPROPERTY(BlueprintReadWrite)
	FVector CalculatedLocation;

	UPROPERTY(BlueprintReadWrite)
	FRotator CalculatedRotation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Drop Effects")
	bool bRotates = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Drop Effects")
	float RotationRate = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Drop Effects")
	bool bSinusoudalMovement = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	float SinePeriodConstant = 1.f; 

	UFUNCTION(BlueprintCallable, Category = "Loot Drop Effects")
	void StartSinusoudalMovement();

	UFUNCTION(BlueprintCallable, Category = "Loot Drop Effects")
	void StartRotation();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Drop Effects")
	float SineAmplitude = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Drop Effects")
	FVector InitialLocation = FVector();
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectApplication = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bApplyEffectsToEnemies = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	float ActorLevel;


	/** Instant **/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TEnumAsByte<EEffectApplicationPolicy> InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	//TArray<TSubclassOf<UGameplayEffect>> MultiInstantGameplayEffectClasses;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	//TEnumAsByte<EEffectApplicationPolicy> MultiInstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	/** Duration **/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TEnumAsByte<EEffectApplicationPolicy> DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	//TArray<TSubclassOf<UGameplayEffect>> MultiDurationGameplayEffectClasses;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	//TEnumAsByte<EEffectApplicationPolicy> MultiDurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	/** Infinite **/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TEnumAsByte<EEffectApplicationPolicy> InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TEnumAsByte<EEffectRemovalPolicy> InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	//TEnumAsByte<EEffectApplicationPolicy> MultiInfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	//TArray<TSubclassOf<UGameplayEffect>> MultiInfiniteGameplayEffectClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<TSubclassOf<UGameplayEffect>> MultiGameplayEffectsClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TEnumAsByte<EEffectApplicationPolicy> MultiGameplayEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TEnumAsByte<EEffectRemovalPolicy> MultiGameplayEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	/** Functions **/
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect>GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

private:


	

	float RunningTime = 0.f;
	
	void ItemMovement(float DeltaTime);
	
public:	


};
