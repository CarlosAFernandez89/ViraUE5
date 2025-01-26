// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISpudObject.h"
#include "PaperZDAnimationComponent.h"
#include "ModularGameplayActors/GSCModularCharacter.h"
#include "VyraNPCCharacterBase.generated.h"

UCLASS(Blueprintable)
class VIRA_API AVyraNPCCharacterBase : public ACharacter, public ISpudObject, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVyraNPCCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UVyraAbilitySystemComponent> VyraAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GASCompanion, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UGSCCoreComponent> GSCCoreComponent;
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UGSCCoreComponent* GetGSCCoreComponent() const {return GSCCoreComponent; }

	//~ Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface
	
public:
	/// GUID required to identify runtime-created objects
	/// This will be generated on save if necessary, or you can initialise it yourself
	UPROPERTY(BlueprintReadOnly)
	FGuid SpudGuid;
};
