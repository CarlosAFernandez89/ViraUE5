// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VyraCompanionPawn.h"
#include "VyraCompanionPawn_WithASC.generated.h"

UCLASS()
class VIRA_API AVyraCompanionPawn_WithASC : public AVyraCompanionPawn
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASC, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UVyraAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASC, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UGSCCoreComponent> GSCCoreComponent;

public:
	AVyraCompanionPawn_WithASC();

protected:
	virtual void PostInitializeComponents() override;
};
