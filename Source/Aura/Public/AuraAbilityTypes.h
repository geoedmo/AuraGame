#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;
USTRUCT(BlueprintType)
struct FDamageEffectParams {

	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag ProjectileDamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;
	
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector Knockback = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false;
	
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAOEDamage = false;
	
};


USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY();

public:

	/*
	*  Getters
	*/
	
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsAOEDamage() const { return bIsAOEDamage; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	float GetDeathImpulseMagnitude() const { return DeathImpulseMagnitude; }
	FVector GetKnockback() const { return Knockback; }
	bool IsSuccessfulKnockback () const { return bIsSuccessfulKnockback; }
	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }
	bool IsRadialDamage() const { return bIsRadialDamage; }
	
	/*
	*  Setters
	*/
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit;}
	void SetIsAOEDamage(bool bInIsAOEDamage) { bIsAOEDamage = bInIsAOEDamage; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsSuccessfulDebuff(bool bInIsSuccessfulDebuff) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }
	void SetDebuffDuration(float InDebuffDuration) { DebuffDuration = InDebuffDuration;}
	void SetDebuffDamage(float InDebuffDamage) { DebuffDamage = InDebuffDamage; }
	void SetDebuffFrequency(float InDebuffFrequency) { DebuffFrequency = InDebuffFrequency; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	void SetDeathImpulse(const FVector& InImpulse) { DeathImpulse = InImpulse; }
	void SetDeathImpulseMagnitude(float InDeathImpulseMagnitude) { DeathImpulseMagnitude = InDeathImpulseMagnitude; }
	void SetKnockback(const FVector& InKnockback) { Knockback = InKnockback; }
	void SetIsSuccessfulKnockback(bool bInIsSuccessfulKnockback) { bIsSuccessfulKnockback = bInIsSuccessfulKnockback; }
	void SetIsRadialDamage(bool bInIsRadialDamage)	{ bIsRadialDamage = bInIsRadialDamage; }
	void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin){ RadialDamageOrigin = InRadialDamageOrigin; }
	void SetRadialDamageOuterRadius(float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius;}
	void SetRadialDamageInnerRadius(float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius;}
	
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}


	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY()
	FVector Knockback = FVector::ZeroVector;

	UPROPERTY()
	bool bIsSuccessfulKnockback = false;

	UPROPERTY()
	bool bIsRadialDamage = false;

	UPROPERTY()
	bool bIsAOEDamage = false;
	
	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;
	
	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;
	
	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
};

template<>
struct TStructOpsTypeTraits< FAuraGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FAuraGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
