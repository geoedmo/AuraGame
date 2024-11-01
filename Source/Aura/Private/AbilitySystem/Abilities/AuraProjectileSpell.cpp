// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "GameplayEffect.h"
#include "GameFramework/ProjectileMovementComponent.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, TSubclassOf<AAuraProjectile> ProjectileType)
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
		ProjectileType,
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

void UAuraProjectileSpell::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	AActor* HomingTarget, bool bOverridePitch, float PitchOverride)
{
		const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();

	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);

	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	
	if (bOverridePitch) Rotation.Pitch = PitchOverride;
	//const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread / 2, FVector::UpVector);
	//const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread / 2, FVector::UpVector);

	const FVector Forward = Rotation.Vector();
	int32 EffectiveNumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);



	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);

		SpawnTransform.SetRotation(Rot.Quaternion());
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn

		);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		// Current Known Issue with Fireballs Homing.. If the target dies before the fireball reaches it, it does not explode.

		
			
		
		if (HomingTarget && HomingTarget->Implements<UCombatInterface>()) {

			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();

		}

		else

		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			//Projectile->HomingTargetSceneComponent->SetCollisionResponseToChannel(ECC_Target, ECollisionResponse::ECR_Ignore);
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;

		}

		//For Projectile Spells, we set the projectile damage type to the damage type, so that it can make use of DamageType specific debuffs.

		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;

		Projectile->DamageEffectParams.DamageType = Projectile->ProjectileDamageType;
				
		Projectile->FinishSpawning(SpawnTransform);
	} 

	/*
	if (NumProjectiles > 1) {

		const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);

		for (int32 i = 0; i < NumProjectiles; i++) {

			const FVector Start = SocketLocation + FVector(0, 0, 10.f);
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Start, Start + Direction * 75.f, 4.f, FColor::Orange, 120.f, 2.f);
			
			// Spawn a projectile on each iteration of the for loop, (probably will need to be a function since it will be repeated below in the else statement.

		}

	}
	else
	{
		// single projectile
		const FVector Start = SocketLocation + FVector(0, 0, 5.f);
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Start, Start + Forward * 75.f, 4.f,	FColor::Orange, 120.f, 2.f);

		//spawn a single projectile
	}


	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Rotation.Vector() * 100.f, 4.f, FColor::White, 120.f, 2.f);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + LeftOfSpread * 100.f, 4.f, FColor::Red, 120.f, 2.f);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + RightOfSpread * 100.f, 4.f, FColor::Green, 120.f, 2.f);	*/
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
