// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInteractableTarget.h"
#include "GameFramework/Actor.h"
#include "InteractableActorBase.generated.h"

UCLASS()
class VIRA_API AInteractableActorBase : public AActor, public IInteractableTarget, public IInteractionInfoAndActions
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractableActorBase();

	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder) override;
	virtual FInteractionOption GetInteractionOption_Implementation() override;
	virtual void OnInteractEvent_Implementation(FGameplayEventData Payload) override;
	virtual void SetInteractionText_Implementation(const FText& InteractionText) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
	void OnInteractWithActor(FGameplayEventData Payload);
	virtual void OnInteractWithActor_Implementation(FGameplayEventData Payload);
	virtual void SetActive_InteractableActor_Implementation(bool bActive = true) override;
	
	UPROPERTY(EditAnywhere, Category = Interaction)
	FInteractionOption Option;

	UPROPERTY(EditAnywhere, Category = Interaction)
	bool bIsActorReadyForInteraction = true;

};
