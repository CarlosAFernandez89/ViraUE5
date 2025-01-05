// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vira/Interaction/IInteractableTarget.h"
#include "Teleporter.generated.h"

UCLASS(Blueprintable)
class VIRA_API ATeleporter : public AActor, public IInteractableTarget, public IInteractionInfoAndActions
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATeleporter();
	
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder) override;
	virtual FInteractionOption GetInteractionOption_Implementation() override;
	virtual void OnInteractEvent_Implementation(FGameplayEventData Payload) override;
	virtual void SetInteractionText_Implementation(const FText& InteractionText) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = Interaction)
	FInteractionOption Option;
public:	
	// Called every frame (you likely don't need this, so it's removed)
	// virtual void Tick(float DeltaTime) override; 

	// The name of the level to teleport to
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport")
	FName LevelToLoad;

	// Collision component to detect overlap
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TeleportCollision;

	// Function to handle the teleportation logic
	UFUNCTION(BlueprintCallable, Category = "Teleport")
	void TeleportPlayer();
};
