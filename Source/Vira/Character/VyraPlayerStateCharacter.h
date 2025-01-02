// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimationComponent.h"
#include "ModularGameplayActors/GSCModularPlayerStateCharacter.h"
#include "Vira/Player/IPlayerInputHelper.h"
#include "VyraPlayerStateCharacter.generated.h"


class UVyraSaveGame_Charms;

UCLASS()
class VIRA_API AVyraPlayerStateCharacter : public AGSCModularPlayerStateCharacter, public IPlayerInputHelper
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GASCompanion, meta = (AllowPrivateAccess = "true"))
	class UGSCCoreComponent* GSCCoreComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GASCompanion, meta = (AllowPrivateAccess = "true"))
	class UGSCAbilityInputBindingComponent* GSCAbilityInputBindingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PaperZD, meta = (AllowPrivateAccess = "true"))
	class UPaperZDAnimationComponent* PaperZDAnimationComponent;
	
	// Name of the Sprite component
	static FName SpriteComponentName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbookComponent> Sprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Charms, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCharmManagerComponent> CharmManagerComponent;

private:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AVyraPlayerStateCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void PostInitializeComponents() override;

	/** Returns Sprite subobject **/
	FORCEINLINE class UPaperFlipbookComponent* GetSprite() const { return Sprite; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UPaperZDAnimInstance* GetPaperZDAnimInstance() const {return PaperZDAnimationComponent->GetAnimInstance(); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UGSCCoreComponent* GetGSCCoreComponent() const {return GSCCoreComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UVyraSaveGame_Charms* GetSaveGameCharms() const {return SaveGame_Charms_Instance;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UCharmManagerComponent* GetCharmManagerComponent() const {return CharmManagerComponent;}
	
	UFUNCTION()
	void LoadSaveFiles();

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void SaveCharms();

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void LoadCharms();

	UFUNCTION()
	void AsyncLoadComplete_Currency(const FString& SlotName, int UserIndex, USaveGame* LoadedGameData);

	UFUNCTION()
	void AsyncLoadComplete_Charms(const FString& SlotName, int UserIndex, USaveGame* LoadedGameData);

private:
	UPROPERTY()
	UVyraSaveGame_Charms* SaveGame_Charms_Instance;
};
