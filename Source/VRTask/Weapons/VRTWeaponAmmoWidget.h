// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VRTWeaponAmmoWidget.generated.h"

UCLASS()
class VRTASK_API UVRTWeaponAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void UpdateAmmo(const int32 NewAmmo);
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmmoTextBlock;

private:
	void UpdateAmmoTextBlock(const FText& NewText);
};
