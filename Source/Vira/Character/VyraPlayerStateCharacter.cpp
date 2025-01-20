// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraPlayerStateCharacter.h"

#include "CharmManagerComponent.h"
#include "PaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "PaperZDAnimationComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/GSCAbilityInputBindingComponent.h"
#include "Components/GSCCoreComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Vira/AbilitySystem/AttributeSets/CombatAttributeSet.h"
#include "Vira/AbilitySystem/AttributeSets/CurrencyAttributeSet.h"
#include "Vira/Player/Components/FloatingCombatText.h"
#include "Vira/System/BlueprintFunctionLibraries/VyraBlueprintFunctionLibrary.h"
#include "Vira/System/SaveGame/VyraSaveGame_Charms.h"
#include "Vira/System/SaveGame/VyraSaveGame_Currency.h"


// Sets default values
AVyraPlayerStateCharacter::AVyraPlayerStateCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), MaxAttackTokensCount(10)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(45.f);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeLocation(FVector(0, 0, -40.f));
	SpringArmComponent->SetRelativeRotation(FRotator(0.0f, -45.f, 0.0f));
	SpringArmComponent->TargetArmLength = 800.f;
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bDoCollisionTest = false;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->FieldOfView = 90.0f;

	FloatingCombatTextComponent = CreateDefaultSubobject<UFloatingCombatText>("FloatingCombatTextComponent");
	GSCCoreComponent = CreateDefaultSubobject<UGSCCoreComponent>("GSCCoreComponent");
	GSCAbilityInputBindingComponent = CreateDefaultSubobject<UGSCAbilityInputBindingComponent>("GSCAbilityInputBindingComponent");
	CharmManagerComponent = CreateDefaultSubobject<UCharmManagerComponent>("CharmManagerComponent");

	bUseControllerRotationYaw = false;
	if(UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		CharacterMovementComponent->bOrientRotationToMovement = false;
		CharacterMovementComponent->bUseControllerDesiredRotation = true;
		CharacterMovementComponent->RotationRate = FRotator(0.0f, -1.f, 0.0f);
	}


	AttackTokensCount = MaxAttackTokensCount;

}

// Called when the game starts or when spawned
void AVyraPlayerStateCharacter::BeginPlay()
{
	Super::BeginPlay();

	LoadSaveFiles();
}

bool AVyraPlayerStateCharacter::ReserveAttackToken_Implementation(int32 Amount)
{
	if(AttackTokensCount >= Amount)
	{
		AttackTokensCount -= Amount;
		return true;
	}

	return false;
}

void AVyraPlayerStateCharacter::ReturnAttackToken_Implementation(int32 Amount)
{
	AttackTokensCount += Amount;
	UKismetMathLibrary::Clamp(AttackTokensCount, 0, MaxAttackTokensCount);;
}

void AVyraPlayerStateCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void AVyraPlayerStateCharacter::NotifyRestarted()
{
	Super::NotifyRestarted();

	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement())
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
		{
			CharacterMovementComponent->MaxWalkSpeed = ASC->GetNumericAttribute(UCombatAttributeSet::GetMovementSpeedAttribute());
		}
	}
}

void AVyraPlayerStateCharacter::SpawnDamageText(AActor* HitActor, const float InDamage, const bool bInCriticalHit)
{
	if (FloatingCombatTextComponent)
	{
		FloatingCombatTextComponent->SpawnFloatingDamageText(HitActor, InDamage, bInCriticalHit);
	}
}

void AVyraPlayerStateCharacter::LoadSaveFiles()
{
	LoadCharms();
}

void AVyraPlayerStateCharacter::SaveCharms()
{
	if(UVyraSaveGame_Charms* SaveGameInstance = Cast<UVyraSaveGame_Charms>(UGameplayStatics::CreateSaveGameObject(UVyraSaveGame_Charms::StaticClass())))
	{
		SaveGame_Charms_Instance = SaveGameInstance;
		
		if(UVyraAbilitySystemComponent* ASC = Cast<UVyraAbilitySystemComponent>(GetAbilitySystemComponent()))
		{
			SaveGameInstance->SaveCharms(CharmManagerComponent);
			FAsyncSaveGameToSlotDelegate SaveGameToSlotDelegate;
			UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex,SaveGameToSlotDelegate);
		}
	}
}

void AVyraPlayerStateCharacter::LoadCharms()
{
	if(UGameplayStatics::DoesSaveGameExist("VyraSaveGame_Charms", 0))
	{
		FAsyncLoadGameFromSlotDelegate LoadedDelegate;
		LoadedDelegate.BindUObject(this, &AVyraPlayerStateCharacter::AsyncLoadComplete_Charms);
		UGameplayStatics::AsyncLoadGameFromSlot("VyraSaveGame_Charms", 0, LoadedDelegate);
	}
}

void AVyraPlayerStateCharacter::AsyncLoadComplete_Currency(const FString& SlotName, int UserIndex, USaveGame* LoadedGameData)
{
	if(const UVyraSaveGame_Currency* SaveGameInstance = Cast<UVyraSaveGame_Currency>(LoadedGameData))
	{
		if(AbilitySystemComponent->GetAttributeSet(UCurrencyAttributeSet::StaticClass()))
		{
			const FSaveGame_Currency LoadedCurrency = SaveGameInstance->LoadCurrency();
			AbilitySystemComponent->SetNumericAttributeBase(UCurrencyAttributeSet::GetSoulsAttribute(), LoadedCurrency.Souls);
		}
	}

}

void AVyraPlayerStateCharacter::AsyncLoadComplete_Charms(const FString& SlotName, int UserIndex,
	USaveGame* LoadedGameData)
{
	if(UVyraSaveGame_Charms* SaveGameInstance = Cast<UVyraSaveGame_Charms>(LoadedGameData))
	{
		// Save so we don't have to load it again.
		SaveGame_Charms_Instance = SaveGameInstance;
		
		if(UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
		{
			FSaveGame_Charms AllCharms = SaveGameInstance->GetAllCharms();
			CharmManagerComponent->SetCharmsOnLoad(AllCharms);
			
			for (auto EquippedCharm : AllCharms.EquippedCharms)
			{
				// 1. Create a new ability spec
				FGameplayAbilitySpec AbilitySpec(EquippedCharm, 1, -1, this); // Level 1, InputID -1 (none), this (source object)

				// 2. Give the ability to the ASC
				FGameplayAbilitySpecHandle GrantedAbilityHandle = ASC->GiveAbility(AbilitySpec);

				// 3. Get the StatTag saved data and set it.
				if (GrantedAbilityHandle.IsValid()) // Check if the ability was granted successfully
				{
					if (FGameplayAbilitySpec* GameplayAbilitySpec  = ASC->FindAbilitySpecFromHandle(GrantedAbilityHandle))
					{
						if(UGameplayAbility* Ability = GameplayAbilitySpec->GetPrimaryInstance())
						{
							if (UVyraGameplayAbility_CharmBase* CharmInstance = Cast<UVyraGameplayAbility_CharmBase>(Ability))
							{
								for (auto StatTag : CharmInstance->CharmAbilityData)
								{
									FGameplayTag TagToFind = FGameplayTag::RequestGameplayTag(StatTag.GameplayTagStackTag.GetTagName());
									TMap<FName, FCharmAbilityData> CharmAbilityDataMap = SaveGameInstance->GetAllCharmTagStacks();
									if(!CharmAbilityDataMap.IsEmpty())
									{
										if(FCharmAbilityData* Data = CharmAbilityDataMap.Find(TagToFind.GetTagName()))
										{
											CharmInstance->SetStatTagStack(TagToFind, Data->GameplayTagStackTagCount);
										}
									}
									else
									{
										UE_LOG(LogTemp, Warning, TEXT("Could not find Tag: %s"), *TagToFind.ToString());
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

