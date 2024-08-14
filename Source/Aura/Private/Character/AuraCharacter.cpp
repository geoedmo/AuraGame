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

#include "UI/WidgetController/AuraWidgetController.h"



AAuraCharacter::AAuraCharacter()
{

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

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

}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InNumLevels)
{
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(InNumLevels);
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

void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AAuraCharacter::InitAbilityActorInfo()
{
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();


	AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController());
	if (AuraPlayerController) {

		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}

	}
	InitializeDefaultAttributes();

}
