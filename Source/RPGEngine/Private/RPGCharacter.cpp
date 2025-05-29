// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCharacter.h"
#include "GAS\RPGAbilitySystemComponent.h"

// Sets default values
ARPGCharacter::ARPGCharacter() :
	abilitySystem(nullptr),
	attributeSet(nullptr),
	characterLevel(1),
	enableTestAbilities(false)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	abilitySystem = CreateDefaultSubobject<URPGAbilitySystemComponent>("AbilitySystemComponent");
	attributeSet = CreateDefaultSubobject<URPGAttributeSet>("AttributeSet");

	if (IsValid(abilitySystem))
	{
		abilitySystem->SetIsReplicated(true);
	}
}

int32 ARPGCharacter::GetCharacterLevel() const
{
	return characterLevel;
}

int32 ARPGCharacter::GetStamina() const
{
	if (!IsValid(attributeSet))
	{
		return 0;
	}
	return attributeSet->GetStamina();
}

int32 ARPGCharacter::GetMaxStamina() const
{
	if (!IsValid(attributeSet))
	{
		return 0;
	}
	return attributeSet->GetMaxStamina();
}

int32 ARPGCharacter::GetHealth() const
{
	if (!IsValid(attributeSet))
	{
		return 0;
	}
	return attributeSet->GetHealth();
}

int32 ARPGCharacter::GetMaxHealth() const
{
	if (!IsValid(attributeSet))
	{
		return 0;
	}
	return attributeSet->GetMaxHealth();
}

int32 ARPGCharacter::GetExperience() const
{
	if (!IsValid(attributeSet))
	{
		return 0;
	}
	return attributeSet->GetExperience();
}

int32 ARPGCharacter::GetMaxExperience() const
{
	if (!IsValid(attributeSet))
	{
		return 0;
	}
	return attributeSet->GetMaxExperience();
}

int32 ARPGCharacter::GetAdrenaline() const
{
	if (!IsValid(attributeSet))
	{
		return 0;
	}
	return attributeSet->GetAdrenaline();
}

int32 ARPGCharacter::GetMaxAdrenaline() const
{
	if (!IsValid(attributeSet))
	{
		return 0;
	}
	return attributeSet->GetMaxAdrenaline();
}

bool ARPGCharacter::ActivateAbilitiesWithTag(FGameplayTagContainer gameplayTags, bool AllowRemoteActivation)
{
	if (!IsValid(abilitySystem))
	{
		return false;
	}
	return abilitySystem->TryActivateAbilitiesByTag(gameplayTags, AllowRemoteActivation);
}

bool ARPGCharacter::ActivateMeleeSwordAbility(bool allowRemote)
{
	if (!IsValid(abilitySystem) && !IsValid(MeleeSwordAbility))
	{
		return false;
	}

	return abilitySystem->TryActivateAbility(MeleeAbilitySpecHandle);
}

void ARPGCharacter::GetActiveAbilitiesWithTag(FGameplayTagContainer abilityTags, TArray<UGameplayAbility*>& abilities, bool MatchExactTag)
{
	if (!IsValid(abilitySystem))
	{
		return;
	}

	TArray<FGameplayAbilitySpec*> matchingAbilities;
	abilitySystem->GetActivatableGameplayAbilitySpecsByAllMatchingTags(abilityTags, matchingAbilities, MatchExactTag);

	for (FGameplayAbilitySpec* spec : matchingAbilities)
	{
		TArray<UGameplayAbility*> abilityInstances = spec->GetAbilityInstances();
		for (UGameplayAbility* active : abilityInstances)
		{
			abilities.Add(active);
		}
	}
}

void ARPGCharacter::ApplyGameplayEffect(TSubclassOf<UGameplayEffect> effect)
{
	if (!IsValid(abilitySystem) || !IsValid(effect))
	{
		return;
	}

	FGameplayEffectContextHandle context = abilitySystem->MakeEffectContext();
	context.AddSourceObject(this);

	FGameplayEffectSpecHandle handle = abilitySystem->MakeOutgoingSpec(effect, characterLevel, context);

	if (handle.IsValid())
	{
		FActiveGameplayEffectHandle activeHandle = abilitySystem->ApplyGameplayEffectSpecToTarget(*handle.Data.Get(), abilitySystem);
	}
}

bool ARPGCharacter::CanApplyGameplayEffect(TSubclassOf<UGameplayEffect> effect)
{
	if (!IsValid(abilitySystem) || !IsValid(effect))
	{
		return false;
	}
	FGameplayEffectContextHandle contextHandle = abilitySystem->MakeEffectContext();
	contextHandle.AddSourceObject(this);
	return abilitySystem->CanApplyAttributeModifiers(effect->GetDefaultObject<UGameplayEffect>(), characterLevel, contextHandle);
}

bool ARPGCharacter::EquipWeapon(ARPGWeapon* Weapon, TEnumAsByte<EWeaponSlot> Slot)
{
	if (!IsValid(Weapon))
	{
		return false;
	}

	switch (Slot)
	{
	case EWeaponSlot::RightHand:
		return EquipRightHand(Weapon);
		break;
	case EWeaponSlot::LeftHand:
		return EquipLeftHand(Weapon);
		break;
	default:
		break;
	}

	return false;
}

bool ARPGCharacter::ActivateAbilityBySlot(TEnumAsByte<EAbilitySlot> Slot, bool AllowRemoteActivation)
{
	if (!IsValid(abilitySystem) || !SlotAbilityHandles.Contains(Slot))
	{
		return false;
	}

	auto SpecHandle = SlotAbilityHandles.Find(Slot);
	return abilitySystem->TryActivateAbility(FGameplayAbilitySpecHandle(*SpecHandle));

}

// Called when the game starts or when spawned
void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ARPGCharacter::setTestAbilities()
{
	if (!IsValid(abilitySystem))
	{
		return;
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		for (TSubclassOf<UGameplayAbility>& TestAbility : TestAbilities)
		{
			abilitySystem->GiveAbility(FGameplayAbilitySpec(TestAbility, GetCharacterLevel(), INDEX_NONE, this));
		}
	}
}

void ARPGCharacter::setMeleeAbilities()
{
	if (!IsValid(abilitySystem))
	{
		return;
	}

	MeleeAbilitySpecHandle = abilitySystem->GiveAbility(FGameplayAbilitySpec(MeleeSwordAbility, GetCharacterLevel(), INDEX_NONE, this));
}

// Called every frame
void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ARPGCharacter::GetAbilitySystemComponent() const
{
	return abilitySystem;
}

void ARPGCharacter::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);
	teamID = FGenericTeamId(Fraction);

	if (!abilitySystem)
	{
		return;
	}

	if (enableTestAbilities)
	{
		setTestAbilities();
	}

	ApplyDefaultAttributeEffects();
	setMeleeAbilities();
}

void ARPGCharacter::HandleHealthChange(float deltaValue, AActor* causer)
{
	OnHealthChange(deltaValue, causer);
	if (GetHealth() <= 0)
	{
		OnDead();
	}
}

void ARPGCharacter::HandleStaminaChange(float deltaValue, AActor* causer)
{
	OnStaminaChange(deltaValue, causer);
}

void ARPGCharacter::HandleAdrenalineChange(float deltaValue, AActor* causer)
{
	OnAdrenalineChange(deltaValue, causer);
}

void ARPGCharacter::HandleExperienceChange(float deltaValue, AActor* causer)
{
	OnExperienceChange(deltaValue, causer);
}

void ARPGCharacter::HandleCharacterLevelUpChange(AActor* causer)
{
	characterLevel++;
	RemoveDefaultAttributeEffects();
	ApplyDefaultAttributeEffects();
	OnCharacterLevelUp(causer);
}

void ARPGCharacter::ApplyDefaultAttributeEffects()
{
	FGameplayEffectContextHandle context = abilitySystem->MakeEffectContext();
	context.AddSourceObject(this);

	for (TSubclassOf<class UGameplayEffect>& defaultEffect : defaultEffects)
	{
		FGameplayEffectSpecHandle handle = abilitySystem->MakeOutgoingSpec(defaultEffect, characterLevel, context);

		if (handle.IsValid())
		{
			FActiveGameplayEffectHandle activeHandle = abilitySystem->ApplyGameplayEffectSpecToTarget(*handle.Data.Get(), abilitySystem);
		}
	}
}

void ARPGCharacter::RemoveDefaultAttributeEffects()
{
	FGameplayEffectQuery query;
	query.EffectSource = this;
	abilitySystem->RemoveActiveEffects(query);
}

void ARPGCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (IsValid(abilitySystem))
	{
		abilitySystem->GetOwnedGameplayTags(TagContainer);
	}
}

FGenericTeamId ARPGCharacter::GetGenericTeamId() const
{
	return teamID;
}

void ARPGCharacter::ClearAbilitySlot(TEnumAsByte<EAbilitySlot> AbilitySlot)
{
	if (SlotAbilityHandles.IsEmpty())
	{
		return;
	}

	if (SlotAbilityHandles.Contains(AbilitySlot))
	{
		auto SpecHandle = SlotAbilityHandles.Find(AbilitySlot);
		abilitySystem->ClearAbility(*SpecHandle);

		SlotAbilityHandles.Remove(AbilitySlot);
	}
}

void ARPGCharacter::AddAbilityToSlot(TSubclassOf<UGameplayAbility> NewAbility, TEnumAsByte<EAbilitySlot> AbilitySlot)
{
	if (!IsValid(abilitySystem))
	{
		return;
	}

	auto SpecHandle = abilitySystem->GiveAbility(FGameplayAbilitySpec(NewAbility, GetCharacterLevel(), INDEX_NONE));

	if (SpecHandle.IsValid())
	{
		SlotAbilityHandles.Add(AbilitySlot, SpecHandle);
	}
}

bool ARPGCharacter::EquipRightHand(ARPGWeapon* Weapon)
{
	if (!IsValid(abilitySystem) || !IsValid(Weapon))
		return false;

	ClearAbilitySlot(EAbilitySlot::LightAttack);
	ClearAbilitySlot(EAbilitySlot::HeavyAttack);
	AddAbilityToSlot(Weapon->LightAttackAbility, EAbilitySlot::LightAttack);
	AddAbilityToSlot(Weapon->HeavyAttackAbility, EAbilitySlot::HeavyAttack);
	return true;
}

bool ARPGCharacter::EquipLeftHand(ARPGWeapon* Weapon)
{
	if (!IsValid(abilitySystem) || !IsValid(Weapon))
		return false;


	ClearAbilitySlot(EAbilitySlot::SecondaryAbility);
	AddAbilityToSlot(Weapon->SecondaryAttackAbility, EAbilitySlot::SecondaryAbility);
	return true;
}
