// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "GameplayEffect.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, TSubclassOf<AAuraProjectile> ProjectileClass)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();

	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);

	FTransform SpawnTransform;

	SpawnTransform.SetLocation(SocketLocation);

	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	//Rotation.Pitch = 0.f;

	SpawnTransform.SetRotation(Rotation.Quaternion());

	// TODO: Set the projectile Rotation

	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);

	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

	//For Projectile Spells, we set the projectile damage type to the damage type, so that it can make use of DamageType specific debuffs.
	Projectile->DamageEffectParams.DamageType = Projectile->ProjectileDamageType;

	Projectile->FinishSpawning(SpawnTransform);

}

/*
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();

	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContextHandle.AddActors(Actors);
	EffectContextHandle.AddInstigator(SourceASC->GetAvatarActor(), Projectile);
	EffectContextHandle.AddOrigin(ProjectileTargetLocation);
	EffectContextHandle.AddSourceObject(this);
	EffectContextHandle.SetAbility(this);
	FHitResult HitResult;
	HitResult.Location = ProjectileTargetLocation;
	EffectContextHandle.AddHitResult(HitResult);

	const FGameplayEffectSpecHandle ProjectileSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();


	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(ProjectileSpecHandle, DamageType, ScaledDamage);

	Projectile->DamageEffectSpecHandle = ProjectileSpecHandle; */
