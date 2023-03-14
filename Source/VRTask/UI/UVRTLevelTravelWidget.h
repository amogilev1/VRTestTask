// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UVRTLevelTravelWidget.generated.h"

class UButton;
class UComboBoxString;

/**
 * Travel to level widget
 */
UCLASS()
class VRTASK_API UUVRTLevelTravelWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ComboBox;

	UPROPERTY(meta = (BindWidget))
	UButton* TravelButton;
	
private:
	void ReadLevels();
	void PopulateComboBox();
	
	UFUNCTION()
    void OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
    
	UFUNCTION()
	void OnTravelButtonPressed();

	FString SelectedLevel;
	TArray<FString> LevelsNames;
};
