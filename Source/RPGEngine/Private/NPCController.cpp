// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCController.h"
#include <Perception\AIPerceptionSystem.h>

ETeamAttitude::Type ANPCController::GetTeamAttitudeTowards(const AActor& otherActor) const
{
	const APawn* OtherPawn = Cast<APawn>(&otherActor);
	FGenericTeamId MyTeamId = GetGenericTeamId();

	if (!IsValid(OtherPawn))
	{
		return ETeamAttitude::Neutral;
	}

	auto CharacterTeamAgent = Cast<IGenericTeamAgentInterface>(&otherActor);
	class IGenericTeamAgentInterface* ControllerTeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());

	if (CharacterTeamAgent == nullptr && ControllerTeamAgent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Teams"));
		return ETeamAttitude::Neutral;
	}

	FGenericTeamId otherTeamId = FGenericTeamId();
	if (CharacterTeamAgent != nullptr)
	{
		otherTeamId = CharacterTeamAgent->GetGenericTeamId();
	}
	else if (ControllerTeamAgent != nullptr)
	{
		otherTeamId = ControllerTeamAgent->GetGenericTeamId();
	}


	ETeamAttitude::Type returnAttitude;
	if (otherTeamId == EFaction::Civilians)
	{
		returnAttitude = ETeamAttitude::Neutral;
	}
	else if (otherTeamId == MyTeamId)
	{
		returnAttitude = ETeamAttitude::Friendly;
	}
	else
	{
		returnAttitude = ETeamAttitude::Hostile;
	}

	return returnAttitude;
}

void ANPCController::SetGenericTeamId(const FGenericTeamId& newTeamId)
{
	Super::SetGenericTeamId(newTeamId);


	UAIPerceptionSystem::GetCurrent(GetWorld())->UpdateListener(*GetAIPerceptionComponent());
}