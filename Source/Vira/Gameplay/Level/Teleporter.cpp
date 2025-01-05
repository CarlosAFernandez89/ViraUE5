// Fill out your copyright notice in the Description page of Project Settings.


#include "Teleporter.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Vira/Character/VyraPlayerStateCharacter.h"
#include "Vira/Messages/VyraVerbMessageHelpers.h"


// Sets default values
ATeleporter::ATeleporter()
{
	// Set this actor to call Tick() every frame (optional, likely not needed here).
	// PrimaryActorTick.bCanEverTick = true;

	// Create the Box Component to handle overlaps
	TeleportCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TeleportCollision"));
	TeleportCollision->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f)); // Adjust size as needed
	RootComponent = TeleportCollision;

	// Set a default level to load (change this to your level's name!)
	LevelToLoad = "YourNewLevelName";
}

void ATeleporter::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}

FInteractionOption ATeleporter::GetInteractionOption_Implementation()
{
	return Option;
}

void ATeleporter::OnInteractEvent_Implementation(FGameplayEventData Payload)
{
	TeleportPlayer();
}

void ATeleporter::SetInteractionText_Implementation(const FText& InteractionText)
{
	Option.Text = InteractionText;
}

// Called when the game starts or when spawned
void ATeleporter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATeleporter::TeleportPlayer()
{
	// Check if the level name is valid
	if (LevelToLoad.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("AMyTeleportActor::TeleportPlayer: LevelToLoad is not set!"));
		return;
	}

	// Perform the level transition
	UGameplayStatics::OpenLevel(this, LevelToLoad);
}

