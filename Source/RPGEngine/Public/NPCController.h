// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RPGCharacter.h"
#include "NPCController.generated.h"

/**
 * 
 */
UCLASS()
class RPGENGINE_API ANPCController : public AAIController
{
	GENERATED_BODY()
	
public:
	void SetGenericTeamId(const FGenericTeamId& newTeamId) override;

protected:
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& otherActor) const override;
};
