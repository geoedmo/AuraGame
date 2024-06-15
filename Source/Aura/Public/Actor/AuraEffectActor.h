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

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AAuraEffectActor();
	

protected:

	virtual void BeginPlay() override;

	/** Variables **/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectRemoval = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
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


public:	


};
