// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "Interaction/CombatInterface.h"
#include "AuraGameplayTags.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Aura/Aura.h"
#include "Actor/AuraProjectile.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1) {
		
		return FString::Printf(TEXT(
			
			//Title
			"<Title>Fire Bolt</>\n"
			
			//Level
			"<Small>Level:</> <Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost:</> <ManaCost>%.1f</>\n"
			//ManaCost
			"<Small>CoolDown:</> <Cooldown>%.1f</><Small> secs</>\n\n"
			
			//Description
			"<Default>This Spell Launches a firebolt at the target, dealing</>"

			//Damage Amount
			
			"<Damage> %d</><Default> "

			//Description
			" fire damage with a chance to burn.</> \n"),

			//Variables
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>Fire Bolt</>\n"
			
			//Level
			"<Small>Level:</> <Level>%d</>\n"
			
			//ManaCost
			"<Small>ManaCost:</> <ManaCost>%.1f</>\n"

			//Cooldown
			"<Small>CoolDown:</> <Cooldown>%.1f</> <Small> secs</>\n\n"

			//Number of Firebolts
			"<Default>This Spell Launches %d bolts of fire"

			//Description
			" at the target dealing "

			//Damage Amount
			"</><Damage>%d</><Default>"
			
			//Description
			" fire damage with a chance to burn.</> \n"),

			//Variables
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			ScaledDamage);

	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);

	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		//Title
		"<Title>Next Level</>\n"

		//Level
		"<Small>Level:</> <Level>%d</>\n"

		//ManaCost
		"<Small>ManaCost:</> <ManaCost>%.1f</>\n"

		//Cooldown
		"<Small>CoolDown:</> <Cooldown>%.1f</><Small> secs</>\n\n"

		//Number of Firebolts
		"<Default>This Spell Launches %d bolts of fire"

		//Description
		" at the target dealing "

		//Damage Amount
		"</><Damage>%d</><Default>"

		//Description
		" fire damage with a chance to burn.</> \n"),

		//Variables
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, NumProjectiles),
		ScaledDamage);

}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, AActor* HomingTarget, bool bOverridePitch, float PitchOverride)
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

			FString MyString3 = HomingTarget->GetRootComponent()->GetName();
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, FString::Printf(TEXT("This is the homing Target if Implements CI: ")) + *MyString3, true);

		}

		else

		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			//Projectile->HomingTargetSceneComponent->SetCollisionResponseToChannel(ECC_Target, ECollisionResponse::ECR_Ignore);
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;

			FString MyString3 = Projectile->HomingTargetSceneComponent->GetName();
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, FString::Printf(TEXT("This is the homing Target if Implements if else: ")) + *MyString3, true);

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
