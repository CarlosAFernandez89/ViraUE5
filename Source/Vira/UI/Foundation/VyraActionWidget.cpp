// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraActionWidget.h"

#include "CommonInputBaseTypes.h"
#include "CommonInputSubsystem.h"
#include "CommonUITypes.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"

FSlateBrush UVyraActionWidget::GetIcon() const
{
	// If there is an Enhanced Input action associated with this widget, then search for any
	// keys bound to that action and display those instead of the default data table settings.
	// This covers the case of when a player has rebound a key to something else
	if (AssociatedInputAction)
	{
		if (const UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = GetEnhancedInputSubsystem())
		{
			TArray<FKey> BoundKeys = EnhancedInputSubsystem->QueryKeysMappedToAction(AssociatedInputAction);
			FSlateBrush SlateBrush;

			if (const UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem())
			{
				return CommonUI::GetIconForEnhancedInputAction(CommonInputSubsystem, AssociatedInputAction);
				//return EnhancedInputAction && CommonUI::IsEnhancedInputSupportEnabled()
				//	? CommonUI::GetIconForEnhancedInputAction(CommonInputSubsystem, AssociatedInputAction)
				//	: CommonUI::GetIconForInputActions(CommonInputSubsystem, InputActions);
			}
		}
	}
	
	return Super::GetIcon();
}

void UVyraActionWidget::SetAssociatedInputAction(UInputAction* InputAction)
{
	AssociatedInputAction = InputAction;
	UpdateActionWidget();
}

void UVyraActionWidget::ForceUpdateActionWidget()
{
	UpdateActionWidget();
}

UEnhancedInputLocalPlayerSubsystem* UVyraActionWidget::GetEnhancedInputSubsystem() const
{
	const UWidget* BoundWidget = DisplayedBindingHandle.GetBoundWidget();
	if (const ULocalPlayer* BindingOwner = BoundWidget ? BoundWidget->GetOwningLocalPlayer() : GetOwningLocalPlayer())
	{
		return BindingOwner->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	}
	return nullptr;
}
