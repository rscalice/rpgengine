// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/RPGAttributeSet.h"
#include <Net/UnrealNetwork.h>
#include <GameplayEffect.h>
#include <GameplayEffectExtension.h>
#include "RPGCharacter.h"

URPGAttributeSet::URPGAttributeSet()
{

}

void URPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(URPGAttributeSet, Health);
	DOREPLIFETIME(URPGAttributeSet, MaxHealth);
	DOREPLIFETIME(URPGAttributeSet, Stamina);
	DOREPLIFETIME(URPGAttributeSet, MaxStamina);
	DOREPLIFETIME(URPGAttributeSet, Adrenaline);
	DOREPLIFETIME(URPGAttributeSet, MaxAdrenaline);
	DOREPLIFETIME(URPGAttributeSet, Experience);
	DOREPLIFETIME(URPGAttributeSet, MaxExperience);
}

void URPGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float DeltaValue = 0.0f;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive)
	{
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	AActor* target = nullptr;
	ARPGCharacter* rpgCharacter = nullptr;

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		target = Data.Target.AbilityActorInfo->AvatarActor.Get();
		rpgCharacter = Cast<ARPGCharacter>(target);
	}
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		if (IsValid(rpgCharacter))
		{
			rpgCharacter->HandleHealthChange(DeltaValue, rpgCharacter);
		}
	}
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		bool staminaOverFlow = GetStamina() > GetMaxStamina() ? FMath::IsNearlyEqual(GetStamina() - DeltaValue, GetMaxStamina()) : false;

		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
		if (IsValid(rpgCharacter) && !staminaOverFlow)
		{
			rpgCharacter->HandleStaminaChange(DeltaValue, rpgCharacter);
		}
	}
	if (Data.EvaluatedData.Attribute == GetAdrenalineAttribute())
	{
		SetAdrenaline(FMath::Clamp(GetAdrenaline(), 0.0f, GetMaxAdrenaline()));
		if (IsValid(rpgCharacter))
		{
			rpgCharacter->HandleAdrenalineChange(DeltaValue, rpgCharacter);
		}
	}
	if (Data.EvaluatedData.Attribute == GetExperienceAttribute())
	{
		float difference = GetExperience() - GetMaxExperience();

		if (IsValid(rpgCharacter))
		{
			rpgCharacter->HandleExperienceChange(difference, rpgCharacter);
			if (difference >= 0)
			{
				rpgCharacter->HandleCharacterLevelUpChange(rpgCharacter);
			}
		}
	}
}

void URPGAttributeSet::PreAttributeChange(const FGameplayAttribute& attribute, float& NewValue)
{
	Super::PreAttributeChange(attribute, NewValue);

	if (attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	if (attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
	}
	if (attribute == GetMaxAdrenalineAttribute())
	{
		AdjustAttributeForMaxChange(Adrenaline, MaxAdrenaline, NewValue, GetAdrenalineAttribute());
	}
	if (attribute == GetMaxExperienceAttribute())
	{
		SetExperience(0);
	}
}

void URPGAttributeSet::OnRep_Health(const FGameplayAttributeData& oldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Health, oldHealth);
}

void URPGAttributeSet::OnRep_Stamina(const FGameplayAttributeData& oldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Stamina, oldStamina);
}

void URPGAttributeSet::OnRep_Adrenaline(const FGameplayAttributeData& oldAdrenaline)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Adrenaline, oldAdrenaline);
}

void URPGAttributeSet::OnRep_Experience(const FGameplayAttributeData& oldExperience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, Health, oldExperience);
}

void URPGAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& oldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxHealth, oldMaxHealth);
}

void URPGAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& oldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxStamina, oldMaxStamina);
}

void URPGAttributeSet::OnRep_MaxAdrenaline(const FGameplayAttributeData& oldMaxAdrenaline)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxAdrenaline, oldMaxAdrenaline);
}

void URPGAttributeSet::OnRep_MaxExperience(const FGameplayAttributeData& oldMaxExperience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSet, MaxExperience, oldMaxExperience);
}

void URPGAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& affectedAttribute, const FGameplayAttributeData& maxAttribute, float newMax, const FGameplayAttribute& affectedAttributeProperty)
{
	UAbilitySystemComponent* component = GetOwningAbilitySystemComponent();
	auto currentMax = maxAttribute.GetCurrentValue();

	if (!FMath::IsNearlyEqual(currentMax, newMax) && component)
	{
		component->ApplyModToAttributeUnsafe(affectedAttributeProperty, EGameplayModOp::Additive, newMax);
	}
}
