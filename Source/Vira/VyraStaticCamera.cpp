// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraStaticCamera.h"

#include "Character/VyraPlayerStateCharacter.h"
#include "Components/BoxComponent.h"
#include "Messages/VyraVerbMessageHelpers.h"


AVyraStaticCamera::AVyraStaticCamera()
{
	OverlapBoxComponent = CreateDefaultSubobject<UBoxComponent>("OverlapBoxComponent");
	OverlapBoxComponent->SetupAttachment(RootComponent);
	OverlapBoxComponent->SetGenerateOverlapEvents(true);
	OverlapBoxComponent->SetLineThickness(15.f);
	OverlapBoxComponent->SetBoxExtent(FVector(200.f, 500.f, 500.f));

	OverlapBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AVyraStaticCamera::OnOverlapBegin_OverlapBoxComponent);
	OverlapBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AVyraStaticCamera::OnOverlapEnd_OverlapBoxComponent);
	
}

void AVyraStaticCamera::OnOverlapBegin_OverlapBoxComponent(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
                                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AVyraPlayerStateCharacter::StaticClass()) && NewViewTarget)
	{
		if (APlayerController* PC = UVyraVerbMessageHelpers::GetPlayerControllerFromObject(OtherActor))
		{
			PC->SetViewTargetWithBlend(NewViewTarget, BlendTime, VTBlend_Cubic);
		}
	}
}

void AVyraStaticCamera::OnOverlapEnd_OverlapBoxComponent(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AVyraPlayerStateCharacter::StaticClass()) && NewViewTarget)
	{
		if (APlayerController* PC = UVyraVerbMessageHelpers::GetPlayerControllerFromObject(OtherActor))
		{
			PC->SetViewTargetWithBlend(OtherActor, BlendTime, VTBlend_Cubic);
		}
	}
}
