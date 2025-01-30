// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Vira/Interaction/IInteractableTarget.h"
#include "VyraNPC.generated.h"

UCLASS()
class VIRA_API AVyraNPC : public ACharacter, public IInteractableTarget, public IInteractionInfoAndActions
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVyraNPC();

	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder) override;

	virtual FInteractionOption GetInteractionOption_Implementation() override;
	virtual void OnInteractEvent_Implementation(FGameplayEventData Payload) override;
	virtual void SetInteractionText_Implementation(const FText& InteractionText) override;

protected:
	UPROPERTY(EditAnywhere)
	FInteractionOption Option;
	
};
