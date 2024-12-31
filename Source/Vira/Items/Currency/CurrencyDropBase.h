// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameFramework/Actor.h"
#include "CurrencyDropBase.generated.h"

UCLASS()
class VIRA_API ACurrencyDropBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;

public:
	// Sets default values for this actor's properties
	ACurrencyDropBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currency Drop")
	float Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Currency Drop")
	TSubclassOf<class UGameplayEffect> GameplayEffectClass;

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Currency Drop")
	void OnCurrencyAcquired();

private:
	UPROPERTY()
	class UAbilitySystemComponent* TargetASC;
};
