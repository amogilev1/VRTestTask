// Fill out your copyright notice in the Description page of Project Settings.


#include "UVRTLevelTravelWidget.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Kismet/GameplayStatics.h"

void UUVRTLevelTravelWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ReadLevels();
	PopulateComboBox();

	ComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::OnComboBoxSelectionChanged);
	TravelButton->OnClicked.AddDynamic(this, &ThisClass::OnTravelButtonPressed);
}

void UUVRTLevelTravelWidget::ReadLevels()
{
	// Get all UWorld derived assets
	TArray<FAssetData> LevelsList;

	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	AssetRegistryModule.Get().GetAssetsByClass(UWorld::StaticClass()->GetFName(), LevelsList, false);
	
	for (auto& Asset : LevelsList)
	{
		const FString AssetPath = Asset.PackageName.ToString();

		// Let's assume that in our naming convention we have a rule that all playable levels names must end with _P
		if (AssetPath.EndsWith("_P"))
		{
			TArray<FString> PathsToAsset;
			AssetPath.ParseIntoArray(PathsToAsset, TEXT("/"));

			LevelsNames.Add(PathsToAsset.Last());
		}
	}
}

void UUVRTLevelTravelWidget::PopulateComboBox()
{
	if (!ComboBox)
	{
		return;
	}

	for (const FString& LevelName : LevelsNames)
	{
		ComboBox->AddOption(LevelName);
	}

	if (ComboBox->GetOptionCount() > 0)
	{
		SelectedLevel = ComboBox->GetOptionAtIndex(0);
		ComboBox->SetSelectedOption(SelectedLevel);
	}
}

void UUVRTLevelTravelWidget::OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	SelectedLevel = SelectedItem;
}

void UUVRTLevelTravelWidget::OnTravelButtonPressed()
{
	UGameplayStatics::OpenLevel(this, FName(SelectedLevel));
}
