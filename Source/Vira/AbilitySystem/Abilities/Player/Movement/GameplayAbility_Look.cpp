// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_Look.h"

#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Vira/Messages/VyraVerbMessageHelpers.h"

UGameplayAbility_Look::UGameplayAbility_Look(): MouseLocationNiagaraSystem(nullptr), CommonInputSubsystem(nullptr),
                                                PlayerController(nullptr),
                                                CurrentMouseHitLocation()
{
}

void UGameplayAbility_Look::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (APlayerController* PC = UVyraVerbMessageHelpers::GetPlayerControllerFromObject(GetAvatarActorFromActorInfo()))
	{
		PlayerController = PC;

		CommonInputSubsystem = PC->GetLocalPlayer()->GetSubsystem<UCommonInputSubsystem>();
	}
}

void UGameplayAbility_Look::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGameplayAbility_Look::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_Look::Look(const FVector2D& MovementInput)
{
	const ECommonInputType InputType = CommonInputSubsystem->GetCurrentInputType();
	if (InputType == ECommonInputType::Gamepad)
	{
		float InputMagnitude = MovementInput.Size();

		// If there's significant input, update rotation
		if (InputMagnitude > 0.1f)
		{
			FVector2D NormalizedInput = MovementInput.GetSafeNormal();

			FRotator TargetRotation = FVector(NormalizedInput.Y, NormalizedInput.X, 0.f).Rotation();
			FRotator CurrentControlRotation = PlayerController->GetControlRotation();
			FRotator DeltaRotation = TargetRotation - CurrentControlRotation;
			DeltaRotation.Normalize();
			
			FRotator SmoothedRotation = CurrentControlRotation + DeltaRotation * GetWorld()->GetDeltaSeconds() * ControllerRotationSpeed;
			
			PlayerController->SetControlRotation(SmoothedRotation);
		}
	}
	else if (InputType == ECommonInputType::MouseAndKeyboard)
	{
		if (RaycastToGround(CurrentMouseHitLocation))
		{
			// Calculate the look-at rotation from the character to the mouse position
			FVector CharacterLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CharacterLocation, CurrentMouseHitLocation);;
			
			// Only use the Yaw component to keep the character upright
			LookAtRotation.Pitch = 0.f;
			LookAtRotation.Roll = 0.f;
			
			PlayerController->SetControlRotation(LookAtRotation);
			DrawMouseLocationToWorld(CurrentMouseHitLocation);
		}
	}
}

bool UGameplayAbility_Look::RaycastToGround(FVector& HitLocation, float TraceDistance)
{

	FVector WorldLocation, WorldDirection;
	if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		// Define the start and end points for the trace
		FVector Start = WorldLocation;
		FVector End = Start + (WorldDirection * TraceDistance);

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetAvatarActorFromActorInfo());

		// Perform the line trace
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
		{
			HitLocation = HitResult.ImpactPoint; // Location where the trace hit
			return true;
		}
	}
	return false;
}

void UGameplayAbility_Look::DrawMouseLocationToWorld_Implementation(const FVector InMouseHitLocation)
{
	if (MouseLocationNiagaraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, MouseLocationNiagaraSystem, InMouseHitLocation);
	}
}
