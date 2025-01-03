// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VyraStaticCamera.generated.h"

UCLASS()
class VIRA_API AVyraStaticCamera : public AActor
{
	GENERATED_BODY()

public:
	AVyraStaticCamera();

protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* OverlapBoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "ViewTarget")
	AActor* NewViewTarget = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "ViewTarget")
	float BlendTime = 0.5f;
	

	UFUNCTION()
	void OnOverlapBegin_OverlapBoxComponent(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd_OverlapBoxComponent(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
