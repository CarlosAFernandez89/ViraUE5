﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActionWidget.h"
#include "VyraActionWidget.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class VIRA_API UVyraActionWidget : public UCommonActionWidget
{
	GENERATED_BODY()
public:

	//~ Begin UCommonActionWidget interface
	virtual FSlateBrush GetIcon() const override;
	//~ End of UCommonActionWidget interface

	UFUNCTION(BlueprintCallable)
	void SetAssociatedInputAction(UInputAction* InputAction);

	/** The Enhanced Input Action that is associated with this Common Input action. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UInputAction> AssociatedInputAction;

private:

	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem() const;
};
