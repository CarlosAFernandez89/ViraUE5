// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISpudObject.h"
#include "GameFramework/Actor.h"
#include "VyraSPUDActor.generated.h"

UCLASS()
class VIRA_API AVyraSPUDActor : public AActor, public ISpudObject
{
	GENERATED_BODY()
};
