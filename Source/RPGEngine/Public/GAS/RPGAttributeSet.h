// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RPGAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * RPG | Character Attribute
 */
UCLASS()
class RPGENGINE_API URPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	URPGAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& attribute, float& NewValue) override;


	UPROPERTY(BlueprintReadOnly, Category = "RPG | Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "RPG | Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "RPG | Attributes", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly, Category = "RPG | Attributes", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly, Category = "RPG | Attributes", ReplicatedUsing = OnRep_Adrenaline)
	FGameplayAttributeData Adrenaline;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Adrenaline);

	UPROPERTY(BlueprintReadOnly, Category = "RPG | Attributes", ReplicatedUsing = OnRep_MaxAdrenaline)
	FGameplayAttributeData MaxAdrenaline;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxAdrenaline);

	UPROPERTY(BlueprintReadOnly, Category = "RPG | Attributes", ReplicatedUsing = OnRep_Experience)
	FGameplayAttributeData Experience;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Experience);

	UPROPERTY(BlueprintReadOnly, Category = "RPG | Attributes", ReplicatedUsing = OnRep_MaxExperience)
	FGameplayAttributeData MaxExperience;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxExperience);


protected:

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& oldHealth);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& oldStamina);

	UFUNCTION()
	virtual void OnRep_Adrenaline(const FGameplayAttributeData& oldAdrenaline);

	UFUNCTION()
	virtual void OnRep_Experience(const FGameplayAttributeData& oldExperience);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& oldMaxHealth);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& oldMaxStamina);

	UFUNCTION()
	virtual void OnRep_MaxAdrenaline(const FGameplayAttributeData& oldMaxAdrenaline);

	UFUNCTION()
	virtual void OnRep_MaxExperience(const FGameplayAttributeData& oldMaxExperience);

	UFUNCTION()
	virtual void AdjustAttributeForMaxChange(const FGameplayAttributeData& affectedAttritbute, const FGameplayAttributeData& maxAttribute, float newMax, const FGameplayAttribute& affectedAttributeProperty);
};
