// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Aura/Aura.h"
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

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());

}

void AAuraProjectile::Destroyed()
{

	if (!bHit && !HasAuthority()) 
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactSystem, GetActorLocation());
		
		if (LoopingSoundComponent) {
		
			LoopingSoundComponent->Stop();
		
		}

		bHit = true;
	}
	
	
	Super::Destroyed();


}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const bool bDamageEffectSpecHandleIsValid = DamageEffectSpecHandle.Data.IsValid();

	if (!bDamageEffectSpecHandleIsValid) return;

	const bool bOverLappingInstigatorIsSelf = DamageEffectSpecHandle.Data.Get()->GetContext().GetInstigator() == OtherActor;
	const bool bEffectCauserIsSelf = DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor;

	if (bEffectCauserIsSelf || bOverLappingInstigatorIsSelf)
	{
		return;
	}
	
	// BE AWARE: This check for enemy friendly fire breaks PVP in the future for overlapping projectiles 

	AActor* InstigatingActor = DamageEffectSpecHandle.Data.Get()->GetContext().GetInstigator();

	if (!UAuraAbilitySystemLibrary::IsNotFriend(InstigatingActor, OtherActor))
	{
		return;
	}

	if (!bHit) {
		
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactSystem, GetActorLocation());

			if (LoopingSoundComponent) {
				LoopingSoundComponent->Stop();
			}
			bHit = true;
	}

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{

			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());

		}

		Destroy();

	} 
	else 
	{
		bHit = true;
	}


}


