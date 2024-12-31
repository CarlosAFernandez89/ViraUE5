// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "VyraCheatManager.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class VIRA_API UVyraCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:
    UVyraCheatManager();

	virtual void InitCheatManager() override;

	UFUNCTION(Exec, BlueprintAuthorityOnly)
	virtual void DamageSelf(const float DamageAmount);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffects")
	TSubclassOf<UGameplayEffect> DamageGE;

protected:

	void ApplySetByCallerDamage(class UVyraAbilitySystemComponent* ASC, float DamageAmount) const;

	class UVyraAbilitySystemComponent* GetVyraAbilitySystemComponent() const;
	
};
