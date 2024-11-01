// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/AuraCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "UI/HUD/AuraHUD.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"
#include "Player/AuraPlayerController.h"
#include "NiagaraComponent.h"
#include "UI/Widget/AuraUserWidget.h"
#include "Components/WidgetComponent.h"
#include "AbilitySystem/Components/DebuffNiagaraComponent.h"
#include "AuraGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/AuraWidgetController.h"



AAuraCharacter::AAuraCharacter()
{

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->bDoCollisionTest = false;

	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	AuraCastBar = CreateDefaultSubobject<UWidgetComponent>("AuraCastBar");
	AuraCastBar->SetupAttachment(GetRootComponent());

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("Level Up Niagara Component");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;



	CharacterClass = ECharacterClass::Elementalist;

}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Fill in PlayerState Since it is Used Extensively Here
	AuraPlayerState = GetPlayerState<AAuraPlayerState>();

	// Initabilityactorinfo for server
	InitAbilityActorInfo();
	AddCharacterAbilities();

}


void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// init for the client
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	check(AuraPlayerState);

	AuraPlayerState->AddToXP(InXP);
}

int32 AAuraCharacter::GetXP_Implementation()
{
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerXP();
}


int32 AAuraCharacter::FindLevelForIncomingXP_Implementation(int32 InXP)
{
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

void AAuraCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent)) {

		// Orientate the Niagara System towards the Player Character's Camera
		const FVector CameraLocation = Camera->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraLocation = (CameraLocation - NiagaraSystemLocation).Rotation();

		LevelUpNiagaraComponent->SetWorldRotation(ToCameraLocation);
		LevelUpNiagaraComponent->Activate(true);
	}

	if (IsValid(LevelUpSound)) {
		UGameplayStatics::PlaySound2D( GetWorld(), LevelUpSound);
	}
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InNumLevels)
{
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(InNumLevels);

	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());

	AuraASC->UpdateAbilityStatuses(AuraPlayerState->GetPlayerLevel());

}

int32 AAuraCharacter::GetAttributePointsRewarded_Implementation(int32 InLevel)
{
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[InLevel].AttributePointAward;
}

int32 AAuraCharacter::GetSpellPointsRewarded_Implementation(int32 InLevel)
{
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[InLevel].SpellPointAward;
}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	check(AuraPlayerState);
	AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	check(AuraPlayerState);
	AuraPlayerState->AddToSpellPoints(InSpellPoints);
}

int32 AAuraCharacter::GetAttributePoints_Implementation()
{
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerAttributePoints();
}

int32 AAuraCharacter::GetSpellPoints_Implementation()
{
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerSpellPoints();
}

void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->ShowMagicCircle(DecalMaterial);
		AuraPlayerController->bShowMouseCursor = false;
	}
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->HideMagicCircle();
		AuraPlayerController->bShowMouseCursor = true;
	}
}

void AAuraCharacter::StunTagChanged(const FGameplayTag StunTag, int32 NewCount)
{
	Super::StunTagChanged(StunTag, NewCount);
}

void AAuraCharacter::OnRep_Stunned()
{

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)) {

		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		FGameplayTagContainer Container;
		Container.AddTag(GameplayTags.Player_Block_CursorTrace);
		Container.AddTag(GameplayTags.Player_Block_InputReleased);
		Container.AddTag(GameplayTags.Player_Block_InputPressed);
		Container.AddTag(GameplayTags.Player_Block_InputHeld);

		if (bIsStunned) {
			AuraASC->AddLooseGameplayTags(Container);
			StunDebuffComponent->Activate();
		}
		else
		{
			AuraASC->RemoveLooseGameplayTags(Container);
			StunDebuffComponent->Deactivate();
		}
	}

}

void AAuraCharacter::OnRep_Burned()
{

	if (bIsBurned) {

		BurnDebuffComponent->Activate();
	}
	else {
		BurnDebuffComponent->Deactivate();
	}

}

void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(AuraCastBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

}

void AAuraCharacter::InitAbilityActorInfo()
{
	AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	OnAscRegistered.Broadcast(AbilitySystemComponent);


	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraCharacter::StunTagChanged);

	AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController());
	if (AuraPlayerController) {

		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}

	}

	InitializeDefaultAttributes();

}


