// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingCombatText.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/Canvas.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Vira/System/BlueprintFunctionLibraries/VyraBlueprintFunctionLibrary.h"
#include "Vira/System/Subsystems/GameSettingsSubsystem.h"
#include "Vira/System/Utils/VyraStringUtils.h"


UFloatingCombatText::UFloatingCombatText(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), FloatingDamageFont(nullptr), CriticalDamageColor(FLinearColor::Yellow), DamageColor(FLinearColor::White),
	  FloatingDamageSystem(nullptr),
	  FloatingDamageMaterial(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;

	NiagaraSystemColorName = FName("FloatingDamageText_LinearColor");
	NiagaraSystemDimensionName = FName("FloatingDamageText_Dimension");
	NiagaraSystemMaterialName = FName("FloatingDamageText_MaterialInterface");
}

void UFloatingCombatText::BeginPlay()
{
	Super::BeginPlay();

	if (UVyraGameInstance* VGA = UVyraBlueprintFunctionLibrary::GetVyraGameInstance(this))
	{
		if (UGameSettingsSubsystem* GSS = VGA->GetSubsystem<UGameSettingsSubsystem>())
		{
			bComponentActive = GSS->GetShowDamageNumbers();
			GSS->OnShowDamageNumbersUpdated.AddDynamic(this, &UFloatingCombatText::OnShowDamageNumbersUpdated);
			bUseSmallNumbers = GSS->GetUseSmallDamageNumbers();
			GSS->OnUseSmallDamageNumbersUpdated.AddDynamic(this, &UFloatingCombatText::OnUseSmallDamageNumbersUpdated);
		}
	}
}

void UFloatingCombatText::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UFloatingCombatText::SpawnFloatingDamageText(AActor* HitActor, const float DamageAmount, const bool bCriticalHit)
{
	if (bComponentActive)
	{
		const FString FloatingDamageText = bUseSmallNumbers ?
			UVyraStringUtils::DamageToShortStringFormater(this, FMath::CeilToFloat(DamageAmount), true, 1)
		  : UVyraStringUtils::DamageToLongStringFormater(this, FMath::CeilToFloat(DamageAmount));
		const FVector2D FloatingDamageDimensions = FVector2D(FloatingDamageText.Len() * 256.f, 128.f);
		const FVector2D FloatingDamageOffset = FloatingDamageDimensions / 2.f;
		const FLinearColor FloatingDamageColor = bCriticalHit ? CriticalDamageColor : DamageColor;

		//Create render target data
		if (UTextureRenderTarget2D* RenderTarget2D = UKismetRenderingLibrary::CreateRenderTarget2D(
			this, FloatingDamageDimensions.X, FloatingDamageDimensions.Y
			, ETextureRenderTargetFormat::RTF_R8))
		{
			if (FloatingDamageMaterial && FloatingDamageFont)
			{
				if (UMaterialInstanceDynamic* MID = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this,FloatingDamageMaterial))
				{
					MID->SetTextureParameterValue(FName("RT_FloatingDamage"), RenderTarget2D);

					UCanvas* CanvasReference;
					FVector2D CanvasSize;
					FDrawToRenderTargetContext DrawRenderTargetContext;
					UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, RenderTarget2D,CanvasReference,CanvasSize, DrawRenderTargetContext);

					CanvasReference->DrawText(FloatingDamageFont, FloatingDamageText, FloatingDamageOffset.X, FloatingDamageOffset.Y);
					UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, DrawRenderTargetContext);

					//Spawn NiagaraParticle
					if (UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FloatingDamageSystem, CalculateSpawnLocation(HitActor)))
					{
						NiagaraComponent->SetVariableLinearColor(NiagaraSystemColorName, FloatingDamageColor);
						NiagaraComponent->SetVariableVec2(NiagaraSystemDimensionName, bCriticalHit ? FloatingDamageDimensions * CriticalDamageSizeMultiplier : FloatingDamageDimensions * NormalDamageSizeMultiplier);
						NiagaraComponent->SetVariableMaterial(NiagaraSystemMaterialName, MID);
						NiagaraComponent->SetVariableFloat(FName("ParticleLifetime"), bCriticalHit ? 1.25f : 0.75f);
					}
				}
			}
		}

		
		
	}
}

void UFloatingCombatText::OnShowDamageNumbersUpdated(const bool InState)
{
	bComponentActive = InState;
}
void UFloatingCombatText::OnUseSmallDamageNumbersUpdated(const bool InState)
{
	bUseSmallNumbers = InState;
}

FVector UFloatingCombatText::CalculateSpawnLocation(AActor* HitActor) const
{
	if (HitActor)
	{
		if (ACharacter* Character = Cast<ACharacter>(HitActor))
		{
			return Character->GetActorLocation() +  FVector(0.f,0.f, Character->GetDefaultHalfHeight() * 1.f + FloatingDamageZOffset);
		}
	}

	return FVector::ZeroVector;
}
