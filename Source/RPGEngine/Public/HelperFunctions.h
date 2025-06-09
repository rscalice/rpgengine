// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HelperFunctions.generated.h"

/**
 * 
 */
UCLASS()
class RPGENGINE_API UHelperFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public: 

	UFUNCTION(BlueprintCallable, Category = "RPG Framework | Helper")
	static AActor* GetNextTarget(const TArray<AActor*>& Targets, const AActor* ReferenceActor, AActor* CurrentTarget, bool bclockwise=true);
};
