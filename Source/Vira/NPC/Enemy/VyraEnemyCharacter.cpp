// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraEnemyCharacter.h"

#include "VyraAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/GSCCoreComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Vira/Items/Currency/CurrencyDropBase.h"


// Sets default values
AVyraEnemyCharacter::AVyraEnemyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), AIController(nullptr), EnemyType(EVyraEnemyType::NORMAL),
	  SpawnItemOffset(FVector(0.f, 0.f, 0.f)),
	  bHasFocusTarget(false),
	  BehaviorTree(nullptr)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AVyraEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AIController = CastChecked<AVyraAIController>(UAIBlueprintHelperLibrary::GetAIController(this));

}

void AVyraEnemyCharacter::SpawnDropTableItems(float DropInterval)
{
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

AAIPatrolRoute* AVyraEnemyCharacter::GetAIPatrolRoute_Implementation()
{
	return PatrolRoute;
}

float AVyraEnemyCharacter::SetAIMovementSpeed_Implementation(EAIMovementSpeed MovementSpeed)
{
	switch (MovementSpeed)
	{
	case Idle: GetCharacterMovement()->MaxWalkSpeed = 0.f; 
		return 0;
	case Patrol: GetCharacterMovement()->MaxWalkSpeed = 200.f; 
		return 200.f;
	case Jog: GetCharacterMovement()->MaxWalkSpeed = 350.f; 
		return 350.f;
	case Chase: GetCharacterMovement()->MaxWalkSpeed = 425.f; 
		return 425.f;
	default: ;
	}
	return 0;
}

void AVyraEnemyCharacter::GetAIIdealRange_Implementation(float& AttackRadius, float& DefendRadius)
{
	AttackRadius = UKismetMathLibrary::RandomFloatInRange(50.f, 125.f);
	DefendRadius = UKismetMathLibrary::RandomFloatInRange(225.f, 450.f);
}

float AVyraEnemyCharacter::GetAICurrentHealth_Implementation()
{
	return GetGSCCoreComponent()->GetHealth();
}

float AVyraEnemyCharacter::GetAIMaxHealth_Implementation()
{
	return GetGSCCoreComponent()->GetMaxHealth();
}

bool AVyraEnemyCharacter::GetAIIsDead_Implementation()
{
	return GetGSCCoreComponent()->GetHealth() <= 0.f;
}

void AVyraEnemyCharacter::AIStoreAttackTokens_Implementation(AActor* AttackTarget, int32 Amount)
{
	if(const int32 Count = ReservedAttackTokens.FindOrAdd(AttackTarget))
	{
		ReservedAttackTokens.Add(AttackTarget, Count + Amount);
	}
}

void AVyraEnemyCharacter::AIReturnAllAttackTokens_Implementation()
{
	if (ReservedAttackTokens.Num() > 0)
	{
		for (const TPair<AActor*, int32>& TokenPair : ReservedAttackTokens)
		{
			if (const IAIHelpers* InteractionInterface = Cast<IAIHelpers>(TokenPair.Key))
			{
				InteractionInterface->Execute_ReturnAttackToken(TokenPair.Key, TokenPair.Value);
			}
		}
        
		ReservedAttackTokens.Empty(); // Assuming we want to clear the map after returning tokens
	}
}

void AVyraEnemyCharacter::CheckFocusTargetState_Implementation()
{
	if(AIController)
	{
		if(AIController->GetFocusActor())
		{
			bHasFocusTarget = true;
		}
		else
		{
			bHasFocusTarget = false;
		}
	}
}

void AVyraEnemyCharacter::NavLink_JumpToDestination_Implementation(FVector Destination)
{
	const FVector RealDestination = FVector(Destination.X, Destination.Y, Destination.Z + 125.f);
	FVector LaunchVelocity;
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this,
		LaunchVelocity,
		GetActorLocation(),
		RealDestination);

	LaunchCharacter(LaunchVelocity,true, true);
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
