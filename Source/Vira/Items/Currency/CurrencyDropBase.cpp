// Fill out your copyright notice in the Description page of Project Settings.


#include "CurrencyDropBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Components/SphereComponent.h"
#include "Vira/Character/VyraPlayerStateCharacter.h"


// Sets default values
ACurrencyDropBase::ACurrencyDropBase(): Quantity(0), TargetASC(nullptr)
{
	// Create a sphere collider
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(25.0f);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SphereComponent->SetGenerateOverlapEvents(true);

	RootComponent = SphereComponent;
}

// Called when the game starts or when spawned
void ACurrencyDropBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACurrencyDropBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (AVyraPlayerStateCharacter* Character = Cast<AVyraPlayerStateCharacter>(OtherActor))
	{
		// Check if the overlapping actor has an ASC
		TargetASC = Character->GetAbilitySystemComponent();
		if (!TargetASC || !GameplayEffectClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid TargetASC or GameplayEffectClass in ACurrencyDropBase"));
			return;
		}

		// Create the GameplayEffectSpec
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContext);

		if (EffectSpecHandle.IsValid())
		{
			FGameplayEffectSpec* Spec = EffectSpecHandle.Data.Get();
			if (Spec)
			{
				// Set the magnitude for the SetByCaller tag
				Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("SetByCaller.Magnitude"), Quantity);

				// Apply the GameplayEffectSpec to the target
				TargetASC->ApplyGameplayEffectSpecToSelf(*Spec);
				OnCurrencyAcquired();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to retrieve the GameplayEffectSpec in ACurrencyDropBase"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid EffectSpecHandle in ACurrencyDropBase"));
		}
	}
}

void ACurrencyDropBase::OnCurrencyAcquired_Implementation()
{
	Destroy();
}
