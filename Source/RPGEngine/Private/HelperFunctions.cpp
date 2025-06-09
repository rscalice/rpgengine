// Fill out your copyright notice in the Description page of Project Settings.


#include "HelperFunctions.h"

AActor* UHelperFunctions::GetNextTarget(const TArray<AActor*>& Targets, const AActor* ReferenceActor, AActor* CurrentTarget, bool bclockwise)
{
	if (Targets.Num() == 0 || !IsValid(CurrentTarget))
	{
		return nullptr;
	}

	TArray<AActor*> SortedTargets = Targets;
	FVector ReferenceLocation = ReferenceActor->GetActorLocation();

	SortedTargets.Sort([&ReferenceLocation, bclockwise](const AActor& First, const AActor& Second)
		{
			FVector LocationFirst = First.GetActorLocation();
			FVector LocationSecond = Second.GetActorLocation();

			float AngleFirst = FMath::Atan2(LocationFirst.Y - ReferenceLocation.Y, LocationFirst.X - ReferenceLocation.X);
			float AngleSecond = FMath::Atan2(LocationSecond.Y - ReferenceLocation.Y, LocationSecond.X - ReferenceLocation.X);

			AngleFirst = FMath::Fmod(AngleFirst + 2 * PI, 2 * PI);
			AngleSecond = FMath::Fmod(AngleSecond + 2 * PI, 2 * PI);

			return bclockwise ? (AngleFirst < AngleSecond) : (AngleFirst > AngleSecond);
		});

	int32 CurrentTargetIndex = SortedTargets.IndexOfByKey(CurrentTarget);

	if (CurrentTargetIndex == INDEX_NONE)
	{
		return nullptr;
	}

	int32 NextTargetIndex = (CurrentTargetIndex + 1) % SortedTargets.Num();
	return SortedTargets[NextTargetIndex];
}
