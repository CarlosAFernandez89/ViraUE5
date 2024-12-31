// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraNPC.h"


// Sets default values
AVyraNPC::AVyraNPC()
{
}

void AVyraNPC::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}

FInteractionOption AVyraNPC::GetInteractionOption_Implementation()
{
	return Option;
}

void AVyraNPC::OnInteractEvent_Implementation(FGameplayEventData Payload)
{
	IInteractionInfoAndActions::OnInteractEvent_Implementation(Payload);
}

void AVyraNPC::SetInteractionText_Implementation(const FText& InteractionText)
{
	Option.Text = InteractionText;
}

