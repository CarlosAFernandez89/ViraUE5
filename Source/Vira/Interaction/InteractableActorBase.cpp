// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActorBase.h"


// Sets default values
AInteractableActorBase::AInteractableActorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AInteractableActorBase::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& InteractionBuilder)
{
	if (bIsActorReadyForInteraction)
	{
		InteractionBuilder.AddInteractionOption(Option);
	}
}

FInteractionOption AInteractableActorBase::GetInteractionOption_Implementation()
{
	return Option;
}

void AInteractableActorBase::OnInteractEvent_Implementation(FGameplayEventData Payload)
{
	if (bIsActorReadyForInteraction)
	{
		OnInteractWithActor(Payload);
	}
}

void AInteractableActorBase::SetInteractionText_Implementation(const FText& InteractionText)
{
	Option.Text = InteractionText;
}

void AInteractableActorBase::SetActive_InteractableActor_Implementation(bool bActive)
{
	bIsActorReadyForInteraction = bActive;
}


void AInteractableActorBase::OnInteractWithActor_Implementation(FGameplayEventData Payload)
{
	
}

// Called when the game starts or when spawned
void AInteractableActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}


