#pragma once

#include "CoreMinimal.h" 	
#include "UObject/NameTypes.h"
#include "GameplayAbilitySkillTreeRuntimeGraph.generated.h"

UCLASS()
class GAMEPLAYABILITYSKILLTREERUNTIME_API UGameplayAbilitySkillTreeRuntimePin : public UObject {
	GENERATED_BODY()

public:
	UPROPERTY()
	FName PinName;

	UPROPERTY()
	FGuid PinId;

	UPROPERTY()
	UGameplayAbilitySkillTreeRuntimePin* Connection = nullptr;
};

UCLASS()
class GAMEPLAYABILITYSKILLTREERUNTIME_API UGameplayAbilitySkillTreeRuntimeNode : public UObject {
	GENERATED_BODY()

public:
	UPROPERTY()
	UGameplayAbilitySkillTreeRuntimePin* InputPin;

	UPROPERTY()
	TArray<UGameplayAbilitySkillTreeRuntimePin*> OutputPins;

	UPROPERTY()
	FVector2D Position;
};

UCLASS()
class GAMEPLAYABILITYSKILLTREERUNTIME_API UGameplayAbilitySkillTreeRuntimeGraph : public UObject {
	GENERATED_BODY()

public:
	UPROPERTY();
	TArray<UGameplayAbilitySkillTreeRuntimeNode*> Nodes;
};