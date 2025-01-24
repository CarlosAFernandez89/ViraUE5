// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vira/Interaction/InteractableActorBase.h"
#include "ArcadeAltar.generated.h"

UCLASS()
class VIRA_API AArcadeAltar : public AInteractableActorBase
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* InteractCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;


public:
	// Sets default values for this actor's properties
	AArcadeAltar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnInteractWithActor_Implementation(FGameplayEventData Payload) override;
};
