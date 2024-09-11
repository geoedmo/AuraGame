// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"	
#include "AuraGameplayTags.h"
#include "AbilitySystem/Components/DebuffNiagaraComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Interaction/EnemyInteraction.h"
#include "Interaction/AuraPlayerInterface.h"
#include "Aura/Aura.h"


AAuraCharacterBase::AAuraCharacterBase()
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffComponent");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = GameplayTags.Debuff_Burn;

	PrimaryActorTick.bCanEverTick = false;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die(const FVector& DeathImpulse)
{

	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	MutlicastHandleDeath(DeathImpulse);

}

void AAuraCharacterBase::MutlicastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	UGameplayStatics::PlaySoundAtLocation(
		this,
		DeathSound,
		GetActorLocation()
	);

	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->AddImpulse(DeathImpulse * 0.1f, NAME_None, true);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);

	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// So the projectiles don't go to a location they home to and not explode when the character mesh is no longer there.
	// Below Settings makes the capsule fall through the floor which has a somewhat desireable effect... since the projectiles are homing to the capsule,
	// they fall into the floor and explode.

	if (!this->Implements<UAuraPlayerInterface>())
	{ 	
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECollisionResponse::ECR_Block);
	Dissolve();

	// Enemy Health Bar falls through the floor, this is the solution:
	RemoveEnemyHealthBar();

	bDead = true;

	BurnDebuffComponent->Deactivate();

	OnDeathDelegate.Broadcast(this);

	//OnDeath.Broadcast(this);
}

void AAuraCharacterBase::RemoveEnemyHealthBar()
{
	if (this->Implements<UEnemyInteraction>()) {
		UWidgetComponent* EnemyHealthBar = IEnemyInteraction::Execute_GetHealthBar(this);
		EnemyHealthBar->SetVisibility(false);
		EnemyHealthBar->DestroyComponent();
	}
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.Get()->AddSourceObject(this);

	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}
void AAuraCharacterBase::AddCharacterAbilities()
{

	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);

	if (!HasAuthority()) return;
	AuraASC->AddCharacterAbilities(StartupAbilities);
	AuraASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);

}
void AAuraCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(
			DissolveMaterialInstance,
			this
		);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);

	}

	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(
			WeaponDissolveMaterialInstance,
			this
		);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}
UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}
FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{	

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon)) {
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}

	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand)) {
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}

	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand)) {
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}

	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail)) {
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	return FVector();
}
bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

FTaggedMontage AAuraCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages) {

		if (TaggedMontage.MontageTag == MontageTag) {
			return TaggedMontage;
		}
	}

	return FTaggedMontage();
}

int32 AAuraCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void AAuraCharacterBase::IncremenetMinionCount_Implementation(int32 Amount)
{
	MinionCount = MinionCount + Amount;
}

ECharacterClass AAuraCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

ACharacter* AAuraCharacterBase::GetACharacter_Implementation()
{
	return this;
}

USkeletalMeshComponent* AAuraCharacterBase::GetWeapon_Implementation()
{
	return Weapon;
}

FOnASCRegistered AAuraCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnAscRegistered;
}

FOnDeathSignature& AAuraCharacterBase::GetOnDeathSignature()
{
	return OnDeathDelegate;
}

/*FOnDeath AAuraCharacterBase::GetOnDeathDelegate()
{
	return OnDeath;
}
*/

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	// Called in InitActorAbilityInfo() on AuraCharacter.cpp

	ApplyEffectToSelf(DefaultPrimaryAttributes, 1);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1);
	ApplyEffectToSelf(DefaultVitalAttributes, 1);

}

