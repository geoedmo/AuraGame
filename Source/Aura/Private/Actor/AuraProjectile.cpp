// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Aura/Aura.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;


	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);


	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}


void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);
	SetReplicateMovement(true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());


}

void AAuraProjectile::Destroyed()
{
	StopLoopingSound();
	if (HomingTargetSceneComponent)	HomingTargetSceneComponent->DestroyComponent();
	if (!bHit && !HasAuthority()) OnHit();
	Super::Destroyed();

}

void AAuraProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactSystem, GetActorLocation());
	StopLoopingSound();
	if (HomingTargetSceneComponent)	HomingTargetSceneComponent->DestroyComponent();
	bHit = true;
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (!IsValidOverlap(OtherActor)) return;
	
	if (!bHit) OnHit();

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			FRotator Rotation = GetActorRotation();
			Rotation.Pitch = 45.f;
			const FVector KnockbackDirection = Rotation.Vector();
			const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackMagnitude;

			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			
			DamageEffectParams.Knockback = KnockbackForce;
			DamageEffectParams.DeathImpulse = DeathImpulse;

			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);

			//TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
			
		}
		if (HomingTargetSceneComponent)	HomingTargetSceneComponent->DestroyComponent();
		Destroy();
	} 

	else
	{
		bHit = true;
		StopLoopingSound();
		if (HomingTargetSceneComponent)	HomingTargetSceneComponent->DestroyComponent();
	}

}

float AAuraProjectile::GetDistanceFromStartOrigin()
{
	FVector CurrentLocationFromOrigin;

	CurrentLocationFromOrigin = StartOrigin - this->GetActorLocation();
	float DistanceFromOrigin = CurrentLocationFromOrigin.Size();

	return DistanceFromOrigin;
}

void AAuraProjectile::StopLoopingSound()
{
	if (LoopingSoundComponent) {
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
}

bool AAuraProjectile::IsValidOverlap(AActor* OtherActor)
{
	if (DamageEffectParams.SourceAbilitySystemComponent == nullptr) return false;
	AActor* SourceAvatar = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatar == OtherActor) return false;
	if (!UAuraAbilitySystemLibrary::IsNotFriend(SourceAvatar, OtherActor)) return false;
	
	return true;
}


