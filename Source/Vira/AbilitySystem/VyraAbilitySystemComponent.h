// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GSCAbilitySystemComponent.h"
#include "Components/GameplayTagStackComponent.h"
#include "VyraAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VIRA_API UVyraAbilitySystemComponent : public UGSCAbilitySystemComponent
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:
	UVyraAbilitySystemComponent();

	UFUNCTION(BlueprintCallable)
	TArray<UGameplayAbility*> GetAllActiveAbilitiesOfClass(TSubclassOf<UGameplayAbility> AbilityClass);

	bool HasAbilityOfClass(TSubclassOf<UGameplayAbility> AbilityClass);

	UFUNCTION(BlueprintCallable)
	bool GiveUniqueAbility(FGameplayAbilitySpecHandle& SpecHandle, TSubclassOf<UGameplayAbility> AbilityClass, int32 Level, int32 InputID);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UGameplayTagStackComponent* GetGameplayTagStackComponent() { return GameplayTagStack; }
	
	template<typename T>
	TArray<T*> GetAllActiveAbilitiesOfClass_Typed();

	
	UFUNCTION(BlueprintCallable)
	void ResetCooldownWithMatchingOwningTag(const FGameplayTagContainer CooldownTagContainer);

	UFUNCTION(BlueprintCallable)
	void ReduceCooldownWithMatchingOwningTag(const FGameplayTagContainer CooldownTagContainer, const float ReductionAmount = 0.5f);
	
	UFUNCTION(BlueprintCallable)
	void AddGameplayTagStack(const FGameplayTag Tag, const int32 Count);
	UFUNCTION(BlueprintCallable)
	void RemoveGameplayTagStack(const FGameplayTag Tag, const int32 Count);
	UFUNCTION(BlueprintCallable)
	void SetGameplayTagStack(const FGameplayTag Tag, const int32 Count);
	UFUNCTION(BlueprintCallable)
	int32 GetGameplayTagStackCount(const FGameplayTag Tag) const;
private:

	void InitializeGameplayTagStack();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="GameplayTagStack", meta = (AllowPrivateAccess = "true"))
	class UGameplayTagStackComponent* GameplayTagStack;
};

template <typename T>
TArray<T*> UVyraAbilitySystemComponent::GetAllActiveAbilitiesOfClass_Typed()
{
	TArray<T*> Abilities;

	for (TArray<FGameplayAbilitySpec> AbilitySpecs = GetActivatableAbilities(); FGameplayAbilitySpec Spec : AbilitySpecs)
	{
		if (UGameplayAbility* SpecAbility = Spec.GetPrimaryInstance())
		{
			if (T* CastedAbility = Cast<T>(SpecAbility))
			{
				Abilities.Add(CastedAbility);
			}
		}
	}

	return Abilities;
}
