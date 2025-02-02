// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "FloatingCombatText.generated.h"


class UNiagaraSystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VIRA_API UFloatingCombatText : public UPawnComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFloatingCombatText(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintCallable, Category="Character|FloatingCombatText")
	void SpawnFloatingDamageText(AActor* HitActor, const float DamageAmount = 1.f, const bool bCriticalHit = false);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|FloatingCombatText")
	UFont* FloatingDamageFont;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|FloatingCombatText")
	float FloatingDamageZOffset = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|FloatingCombatText")
	FLinearColor CriticalDamageColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|FloatingCombatText")
	float CriticalDamageSizeMultiplier = 1.25f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|FloatingCombatText")
	FLinearColor DamageColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|FloatingCombatText|Niagara")
	UNiagaraSystem* FloatingDamageSystem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|FloatingCombatText|Niagara")
	UMaterialInterface* FloatingDamageMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|FloatingCombatText|Niagara")
	FName NiagaraSystemColorName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|FloatingCombatText|Niagara")
	FName NiagaraSystemDimensionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character|FloatingCombatText|Niagara")
	FName NiagaraSystemMaterialName;

	
private:
	UFUNCTION()
	void OnShowDamageNumbersUpdated(bool InState);

	UFUNCTION()
	void OnUseSmallDamageNumbersUpdated(bool InState);

	UFUNCTION()
	FVector CalculateSpawnLocation(AActor* HitActor) const;

	UPROPERTY()
	bool bComponentActive = true;

	UPROPERTY()
	bool bUseSmallNumbers = true;
};
