// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimationComponent.h"
#include "ModularGameplayActors/GSCModularCharacter.h"
#include "VyraNPCCharacterBase.generated.h"

UCLASS()
class VIRA_API AVyraNPCCharacterBase : public AGSCModularCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GASCompanion, meta = (AllowPrivateAccess = "true"))
	class UGSCCoreComponent* GSCCoreComponent;

	// Name of the Sprite component
	static FName SpriteComponentName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbookComponent> Sprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PaperZD, meta = (AllowPrivateAccess = "true"))
	class UPaperZDAnimationComponent* PaperZDAnimationComponent;

public:
	// Sets default values for this character's properties
	AVyraNPCCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	/** Returns Sprite subobject **/
	FORCEINLINE class UPaperFlipbookComponent* GetSprite() const { return Sprite; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UPaperZDAnimInstance* GetPaperZDAnimInstance() const {return PaperZDAnimationComponent->GetAnimInstance(); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UGSCCoreComponent* GetGSCCoreComponent() const {return GSCCoreComponent; }

	
	virtual void PostInitializeComponents() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
