// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraEnemyCharacter.h"

#include "Kismet/KismetMathLibrary.h"
#include "Vira/Items/Currency/CurrencyDropBase.h"


// Sets default values
AVyraEnemyCharacter::AVyraEnemyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), EnemyType(EVyraEnemyType::NORMAL), SpawnItemOffset(FVector(0.f, 0.f, 0.f))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AVyraEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AVyraEnemyCharacter::SpawnDropTableItems(float DropInterval)
{
	// Make sure we don't have an existing timer running.
	if (const UWorld* World = GetWorld())
	{
		for (FCurrencyDropData CurrencyDrop : DropTable.CurrencyDrops)
		{
			TSubclassOf<ACurrencyDropBase> Class = CurrencyDrop.DropClass;
			int32 DropQuant = CurrencyDrop.Quantity;
			FTimerHandle TempTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TempTimerHandle, [this, Class, DropQuant]()
			{
				SpawnItemActor(Class, DropQuant);
			}, UKismetMathLibrary::RandomFloatInRange(0.f, DropInterval), false);
		}
	}
}

void AVyraEnemyCharacter::SpawnItemActor(const TSubclassOf<ACurrencyDropBase>& DropClass, int32 Quantity)
{
	FVector SpawnLocation = GetActorLocation() + SpawnItemOffset;
	FRotator SpawnRotation = FRotator(0, 0, UKismetMathLibrary::RandomFloatInRange(RotationRollMin,RotationRollMax));
	if (UWorld* World = GetWorld())
	{
		if (ACurrencyDropBase* CurrencyDrop = World->SpawnActor<ACurrencyDropBase>(DropClass, SpawnLocation, SpawnRotation))
		{
			CurrencyDrop->UpdateVisualsDueToQuantityChange(Quantity);
		}
	}
}
