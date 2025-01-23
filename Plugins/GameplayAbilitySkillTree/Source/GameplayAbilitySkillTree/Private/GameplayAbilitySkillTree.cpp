// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplayAbilitySkillTree.h"

#include "EdGraphUtilities.h"
#include "GameplayAbilitySkillTreeAssetAction.h"
#include "IAssetTools.h"
#include "SGraphPin.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "FGameplayAbilitySkillTreeModule"


class SSkillTreeCustomGraphPin : public SGraphPin {
public:
	SLATE_BEGIN_ARGS(SSkillTreeCustomGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InnGraphPinObject)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InnGraphPinObject);
	}
	
protected:
	virtual FSlateColor GetPinColor() const override
	{
		return FSlateColor(FLinearColor(0.2f, 1.0f, 0.2f));
	}
};

struct FSkillTreeCustomPinFactory : public FGraphPanelPinFactory
{
public:
	virtual ~FSkillTreeCustomPinFactory() {}
	
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* Pin) const override
	{
		if (FName(TEXT("CustomPin")) == Pin->PinType.PinSubCategory)
		{
			return SNew(SSkillTreeCustomGraphPin, Pin);
		}
		return nullptr;
	}
};
void FGameplayAbilitySkillTreeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	IAssetTools& AssetToolsModule = IAssetTools::Get();
	EAssetTypeCategories::Type AssetType = AssetToolsModule.RegisterAdvancedAssetCategory(FName("GameplayAbilitySkillTree"), FText::FromString("Gameplay Ability Skill Tree"));
	TSharedPtr<FGameplayAbilitySkillTreeAssetAction> AssetAction = MakeShareable(new FGameplayAbilitySkillTreeAssetAction(AssetType));
	AssetToolsModule.RegisterAssetTypeActions(AssetAction.ToSharedRef());

	StyleSetStyle = MakeShareable(new FSlateStyleSet(TEXT("GameplayAbilitySkillTreeStyle")));
	TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("GameplayAbilitySkillTree"));
	FString ContentDirectory = Plugin->GetContentDir();
	StyleSetStyle->SetContentRoot(ContentDirectory);

	FSlateImageBrush* ThumbnailBrush = new FSlateImageBrush(ContentDirectory / TEXT("Icons/Vyra_ConceptArt.png"), FVector2D(128.f, 128.f));
	FSlateImageBrush* IconBrush = new FSlateImageBrush(ContentDirectory / TEXT("Icons/Vyra_ConceptArt.png"), FVector2D(128.f, 128.f));

	FSlateImageBrush* NodeAddPinIcon = new FSlateImageBrush(ContentDirectory / TEXT("Icons/NodeAddPinIcon.png"), FVector2D(128.0f, 128.0f));
	FSlateImageBrush* NodeDeletePinIcon = new FSlateImageBrush(ContentDirectory / TEXT("Icons/NodeDeletePinIcon.png"), FVector2D(128.0f, 128.0f));
	FSlateImageBrush* NodeDeleteNodeIcon = new FSlateImageBrush(ContentDirectory / TEXT("Icons/NodeDeleteNodeIcon.png"), FVector2D(128.0f, 128.0f));

	StyleSetStyle->Set(TEXT("ClassThumbnail.GameplayAbilitySkillTreeAsset"), ThumbnailBrush);
	StyleSetStyle->Set(TEXT("ClassIcon.GameplayAbilitySkillTreeAsset"), IconBrush);
	
	StyleSetStyle->Set(TEXT("GameplayAbilitySkillTree.NodeAddPinIcon"), NodeAddPinIcon);
	StyleSetStyle->Set(TEXT("GameplayAbilitySkillTree.NodeDeletePinIcon"), NodeDeletePinIcon);
	StyleSetStyle->Set(TEXT("GameplayAbilitySkillTree.NodeDeleteNodeIcon"), NodeDeleteNodeIcon);
	
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSetStyle);

	// Register a custom pin factory for creating pins the way we want
	PinFactory = MakeShareable(new FSkillTreeCustomPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(PinFactory);
}

void FGameplayAbilitySkillTreeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSetStyle);
	FEdGraphUtilities::UnregisterVisualPinFactory(PinFactory);
	PinFactory = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGameplayAbilitySkillTreeModule, GameplayAbilitySkillTree)