// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GAS/RPGAttributeSet.h"
#include "GameFramework/Character.h"
#include "RPGCharacter.generated.h"

UCLASS()
class RPGENGINE_API ARPGCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCharacter();

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual int32 GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual int32 GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual int32 GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual int32 GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual int32 GetExperience() const;

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual int32 GetMaxExperience() const;

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual int32 GetAdrenaline() const;

	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual int32 GetMaxAdrenaline() const;

	UFUNCTION(BlueprintCallable, Category = "RPG Abilities")
	bool ActivateAbilitiesWithTag(FGameplayTagContainer gameplayTags, bool AllowRemoteActivation = true);

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG Responses")
	void OnHealthChange(float deltaValue, AActor* causer);

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG Responses")
	void OnStaminaChange(float deltaValue, AActor* causer);

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG Responses")
	void OnAdrenalineChange(float deltaValue, AActor* causer);

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG Responses")
	void OnExperienceChange(float deltaValue, AActor* causer);

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG Responses")
	void OnCharacterLevelUp(AActor* causer);

	UFUNCTION(BlueprintImplementableEvent, Category = "RPG Responses")
	void OnDead();


	UFUNCTION(BlueprintCallable, Category = "RPG Abilities|Melee|Sword")
	bool ActivateMeleeSwordAbility(bool allowRemote = true);

	UFUNCTION(BlueprintCallable, Category = "RPG Abilities|Melee")
	virtual void GetActiveAbilitiesWithTag(FGameplayTagContainer abilityTags, TArray<UGameplayAbility*>& abilities, bool MatchExactTag);


	UFUNCTION(BlueprintCallable, Category = "RPG Abilities")
	virtual void ApplyGameplayEffect(TSubclassOf<UGameplayEffect> effect);


	UFUNCTION(BlueprintCallable, Category = "RPG Abilities")
	virtual bool CanApplyGameplayEffect(TSubclassOf<UGameplayEffect> effect);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class UAbilitySystemComponent* abilitySystem;

	UPROPERTY(EditAnywhere, Category = "RPG Attributes")
	class URPGAttributeSet* attributeSet;

	UPROPERTY(EditAnywhere, Category = "RPG Attributes")
	int32 characterLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Attributes|Debug")
	TArray<TSubclassOf<class UGameplayEffect>> defaultEffects;

	UPROPERTY(EditAnywhere, Category = "RPG Abilities|Debug")
	bool enableTestAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Abilities|Debug")
	TArray<TSubclassOf<class UGameplayAbility>> TestAbilities;

	virtual void setTestAbilities();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG Abilities|Melee|Sword")
	TSubclassOf<class UGameplayAbility> MeleeSwordAbility;

	FGameplayAbilitySpecHandle MeleeAbilitySpecHandle;

	virtual void setMeleeAbilities();

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Inherited via IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PossessedBy(AController* newController) override;
	virtual void HandleHealthChange(float deltaValue, AActor* causer);
	virtual void HandleStaminaChange(float deltaValue, AActor* causer);
	virtual void HandleAdrenalineChange(float deltaValue, AActor* causer);
	virtual void HandleExperienceChange(float deltaValue, AActor* causer);
	virtual void HandleCharacterLevelUpChange(AActor* causer);

	virtual void ApplyDefaultAttributeEffects();
	virtual void RemoveDefaultAttributeEffects();

	// Inherited via IGameplayTagAssetInterface
	void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
};
