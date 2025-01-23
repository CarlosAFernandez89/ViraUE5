#include "GameplayAbilitySkillTreeAssetAction.h"

#include "GameplayAbilitySkillTreeApp.h"
#include "GameplayAbilitySkillTreeAsset.h"

FGameplayAbilitySkillTreeAssetAction::FGameplayAbilitySkillTreeAssetAction(
	EAssetTypeCategories::Type InAssetCategory)
{
	AssetCategory = InAssetCategory;
}

FText FGameplayAbilitySkillTreeAssetAction::GetName() const
{
	return FText::FromString("GameplayAbilitySkillTreeAsset");
}

FColor FGameplayAbilitySkillTreeAssetAction::GetTypeColor() const
{
	return FColor::Green;
}

UClass* FGameplayAbilitySkillTreeAssetAction::GetSupportedClass() const
{
	return UGameplayAbilitySkillTreeAsset::StaticClass();
}

void FGameplayAbilitySkillTreeAssetAction::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* Object : InObjects)
	{
		if (UGameplayAbilitySkillTreeAsset* Asset = Cast<UGameplayAbilitySkillTreeAsset>(Object))
		{
			TSharedRef<FGameplayAbilitySkillTreeApp> Editor(new FGameplayAbilitySkillTreeApp());
			Editor->InitEditor(Mode, EditWithinLevelEditor, Asset);
		}
	}
}

uint32 FGameplayAbilitySkillTreeAssetAction::GetCategories()
{
	return AssetCategory;
}
