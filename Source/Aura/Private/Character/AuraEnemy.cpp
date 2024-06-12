// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

AAuraEnemy::AAuraEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAuraEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAuraEnemy::HighlightActor()
{

	bDrawDebugSphere = true;

}

void AAuraEnemy::UnhighlightActor()
{
	bDrawDebugSphere = false;

}
