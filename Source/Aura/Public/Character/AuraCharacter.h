// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/AuraPlayerInterface.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
class UCameraComponent;
class AAuraPlayerState;
class USpringArmComponent;
class UNiagaraComponent;
class USoundBase;
class UWidgetComponent;

UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IAuraPlayerInterface
{

	GENERATED_BODY()

public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** <Combat Interface> **/

	virtual int32 GetPlayerLevel_Implementation() override;

	/** </Combat Interface> **/

	/* <Player Interface> */
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual int32 GetXP_Implementation() override;
	virtual int32 FindLevelForIncomingXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual void AddToPlayerLevel_Implementation(int32 InNumLevels) override;
	virtual int32 GetAttributePointsRewarded_Implementation(int32 InLevel) override;
	virtual int32 GetSpellPointsRewarded_Implementation(int32 InLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual int32 GetAttributePoints_Implementation() override;
	virtual int32 GetSpellPoints_Implementation() override;
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial = nullptr) override;
	virtual void HideMagicCircle_Implementation() override;
	/* </PlayerInterface> */

	virtual void StunTagChanged(const FGameplayTag StunTag, int32 NewCount)	override;

	virtual void OnRep_Stunned() override;

	virtual void OnRep_Burned() override;

	UPROPERTY()
	TObjectPtr<AAuraPlayerState> AuraPlayerState;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LevelUp")
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "LevelUp")
	TObjectPtr<USoundBase> LevelUpSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> AuraCastBar;


private:

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;

	UPROPERTY(EditAnywhere, Category = Settings)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = Settings)
	USpringArmComponent* SpringArm;

	
};
