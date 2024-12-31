// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraPlaySound_AnimNotify.h"

#include "Kismet/GameplayStatics.h"

UVyraPlaySound_AnimNotify::UVyraPlaySound_AnimNotify(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bFollow(0)
{
	VolumeMultiplier = 1.f;
	PitchMultiplierMin = 1.f;
	PitchMultiplierMax = 1.f;

#if WITH_EDITORONLY_DATA
	Color = FColor(196, 142, 255, 255);
	bPreviewIgnoreAttenuation = false;
#endif // WITH_EDITORONLY_DATA
}

void UVyraPlaySound_AnimNotify::OnReceiveNotify_Implementation(UPaperZDAnimInstance* OwningInstance) const
{
	// We use the SequenceRenderComponent associated to the AnimSequence to know where and how to spawn the sound.
	if (SequenceRenderComponent.IsValid() && Sound)
	{
		if (!Sound->IsOneShot())
		{
			UObject* AnimSequencePkg = GetContainingAsset();
			UE_LOG(LogAudio, Warning, TEXT("PlaySound notify: Anim %s tried to spawn infinitely looping sound asset %s. Spawning suppressed."), *GetNameSafe(AnimSequencePkg), *GetNameSafe(Sound));
			return;
		}

		const float PitchMultiplier = FMath::RandRange(PitchMultiplierMin, PitchMultiplierMax);

#if WITH_EDITORONLY_DATA
		UWorld* World = GetWorld();
		if (bPreviewIgnoreAttenuation && World && World->WorldType == EWorldType::EditorPreview)
		{
			UGameplayStatics::PlaySound2D(World, Sound, VolumeMultiplier, PitchMultiplier);
		}
		else
#endif
		{
			if (bFollow)
			{
				UGameplayStatics::SpawnSoundAttached(Sound, SequenceRenderComponent.Get(), AttachName, FVector(ForceInit), EAttachLocation::SnapToTarget, false, VolumeMultiplier, PitchMultiplier);
			}
			else
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, SequenceRenderComponent->GetComponentLocation(), VolumeMultiplier, PitchMultiplier);
			}
		}
	}
}

FName UVyraPlaySound_AnimNotify::GetDisplayName_Implementation() const
{
	if (Sound)
	{
		return FName(*Sound->GetName());
	}
	else
	{
		return Super::GetDisplayName_Implementation();
	}
}
