// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VyraSPUDActor.h"
#include "VyraSPUDActorRuntime.generated.h"

UCLASS()
class VIRA_API AVyraSPUDActorRuntime : public AVyraSPUDActor , public ISpudObjectCallback
{
	GENERATED_BODY()
};
