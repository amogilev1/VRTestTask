// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "VRTPickupsSettings.generated.h"

/**
 *	Settings that intended to store information about from where we should collect data about pickups
 *	In the test task the only option is to collect from data table, however it may be extended
 *	to be able to collect from different sources (e.g. from the server in JSON format etc.)
 */

class UDataTable;
UCLASS(config = VRTPickups, defaultconfig, meta = (DisplayName = "Pickups Settings"))
class VRTASK_API UVRTPickupsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditAnywhere, Category="Pickups settings")
	FString SourceDataTable;
};
