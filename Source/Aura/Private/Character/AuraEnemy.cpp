// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "UI/Widget/AuraUserWidget.h"
#include "Aura/Aura.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/AuraAIController.h"
#include "Components/WidgetComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AuraGameplayTags.h"

AAuraEnemy::AAuraEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;



	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetGenerateOverlapEvents(true);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	EnemyHealthBar = CreateDefaultSubobject<UWidgetComponent>("EnemyHealthBar");
	EnemyHealthBar->SetupAttachment(GetRootComponent());

	BaseWalkSpeed = 250.f;
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	AuraAIController = Cast<AAuraAIController>(NewController);
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTree);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

void AAuraEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 AAuraEnemy::GetPlayerLevel_Implementation()
{
	return Level;
}

void AAuraEnemy::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpan);
	if (AuraAIController) AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	Super::Die(DeathImpulse);
	SpawnLoot();
}

void AAuraEnemy::FinishSummoning_Implementation()
{
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("CastingSummon"), false);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	check(AbilitySystemComponent)

		InitAbilityActorInfo();

	if (HasAuthority())
	{
	// Give startup abilities from the library, which grab from the data asset.
	UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}

	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(EnemyHealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	// Broadcast health and mAx health changing.
	if (const UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AAuraEnemy::HitReactTagChanged
		);

		// Cast Tag Recieved Callback
		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Player_Casting,EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AAuraEnemy::ReceiveCastDurationFromGameEffect 
		);


		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());

	}


}
void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;

	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void AAuraEnemy::StunTagChanged(const FGameplayTag StunTag, int32 NewCount)
{
	Super::StunTagChanged(StunTag, NewCount);
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"), bIsStunned);
	}
}

void AAuraEnemy::ReceiveCastDurationFromGameEffect(const FGameplayTag CastTag, int32 NewCount)
{
	if (NewCount > 0) 
	{
		UAuraAbilitySystemComponent* AuraASC = Cast <UAuraAbilitySystemComponent>(AbilitySystemComponent);
		if (HasAuthority()) CastTime = AuraASC->GetGameplayEffectDurationBasedOnIfTagHeld(CastTag);
		CastTimeDelegate.Broadcast(CastTime);
	}

}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->AbilityActorInfoSet();


	if (HasAuthority()) {
	// Grab the Default Attributes, which pull from the class info data asset
	InitializeDefaultAttributes();
	}

	OnAscRegistered.Broadcast(AbilitySystemComponent);

	AuraASC->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraEnemy::StunTagChanged);

}

void AAuraEnemy::InitializeDefaultAttributes() const
{

	// Initialize these class defaults which pull from the data asset, and inside use gameplay effects to set attributes at the start
	UAuraAbilitySystemLibrary::InitializeClassDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAuraEnemy::HighlightActor_Implementation()
{
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);

}

void AAuraEnemy::UnhighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);

}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}
