// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"
#include "AuraCharacterBase.generated.h"



class UAbilitySystemComponent;
class UGameplayEffect;
class UAttributeSet;
class UGameplayAbility;
class UAnimMontage;
class UMaterialInstance;
class UNiagaraSystem;
class UDebuffNiagaraComponent;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	
	AAuraCharacterBase();
	
	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* <AbilitySystemInterface> */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	/* </AbilitySystemInterface> */

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	UPROPERTY(ReplicatedUsing = OnRep_Stunned, BlueprintReadOnly, Category = "Combat")
	bool bIsStunned = false;

	UPROPERTY(ReplicatedUsing = OnRep_Burned, BlueprintReadOnly, Category = "Combat")
	bool bIsBurned = false;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	bool bIsBeingShocked = false;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	bool bIsCasting = false;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Casting")
	float CastTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 600.f;

	UFUNCTION()
	virtual void OnRep_Stunned();

	UFUNCTION()
	virtual void OnRep_Burned();

	/* <CombatInterface> */

	//  ** Boolean Setters/Getters (Mostly for Animation States/Casting)
	virtual bool GetIsBeingShocked_Implementation() override;
	virtual void SetIsBeingShocked_Implementation(bool InIsBeingShocked) override;
	virtual bool GetIsCasting_Implementation() override;
	virtual void SetIsCasting_Implementation(bool InIsCasting) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;

	virtual void Die(const FVector& DeathImpulse) override;
	virtual FOnDeathSignature& GetOnDeathSignature() override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MutlicastHandleDeath(const FVector& DeathImpulse);
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void IncremenetMinionCount_Implementation(int32 Amount) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual ACharacter* GetACharacter_Implementation() override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	//virtual FOnDeath GetOnDeathDelegate() override;

	virtual void ReceiveCastDurationFromGameEffect(const FGameplayTag CastTag, int32 NewCount);

	virtual FOnDamageSignature& GetOnDamageSignature() override;
	
	/* </CombatInterface> */
	
	FOnDeathSignature OnDeathDelegate;
	FOnASCRegistered OnAscRegistered;
	FOnDamageSignature OnDamageDelegate;
	
	//FOnDeath OnDeath;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<UNiagaraSystem> BloodEffect;

	UPROPERTY(BlueprintReadOnly)
	bool bDead = false;

	UPROPERTY(EditAnywhere, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = Combat)
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, Category = Combat)
	FName LeftHandSocketName;

	UPROPERTY(EditAnywhere, Category = Combat)
	FName RightHandSocketName;

	UPROPERTY(EditAnywhere, Category = Combat)
	FName TailSocketName;

	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<USoundBase> DeathSound;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;

	virtual void InitializeDefaultAttributes() const;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;

	void AddCharacterAbilities();

	/* Dissolve Effects */
	void Dissolve();
	void RemoveEnemyHealthBar();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	virtual void StunTagChanged(const FGameplayTag StunTag, int32 NewCount);

	/* Minions */

	int32 MinionCount = 0;
	bool CastingSummon = false;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> HaloOfProectionNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;
	
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray <TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray <TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

};
