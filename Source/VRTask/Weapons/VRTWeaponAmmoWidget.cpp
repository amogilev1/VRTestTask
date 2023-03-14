// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTWeaponAmmoWidget.h"

#include "Components/TextBlock.h"

void UVRTWeaponAmmoWidget::UpdateAmmo(const int32 NewAmmo)
{
	UpdateAmmoTextBlock(FText::FromString(FString::FromInt(NewAmmo)));
}

void UVRTWeaponAmmoWidget::UpdateAmmoTextBlock(const FText& NewText)
{
	AmmoTextBlock->SetText(NewText);
}
