// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadeAltar.h"

#include "Components/BoxComponent.h"


// Sets default values
AArcadeAltar::AArcadeAltar()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractCollision = CreateDefaultSubobject<UBoxComponent>("InteractCollision");
	InteractCollision->SetBoxExtent(FVector(100.0f, 100.0f, 200.0f));
	InteractCollision->SetCollisionProfileName("Interactable_OverlapDynamic");
	InteractCollision->CanCharacterStepUpOn = ECB_Owner;
	RootComponent = InteractCollision;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AArcadeAltar::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArcadeAltar::OnInteractWithActor_Implementation(FGameplayEventData Payload)
{
	Super::OnInteractWithActor_Implementation(Payload);
}
