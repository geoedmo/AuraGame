// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{

	GENERATED_BODY()

public:
	AAuraCharacter();

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = Settings)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = Settings)
	USpringArmComponent* SpringArm;

	
};
