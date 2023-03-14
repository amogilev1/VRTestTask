// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InventoryData.h"
#include "VRTPickupsSubsystem.generated.h"

/**
 * Subsystem that deals collection, requesting data about pickups
 */

UCLASS()
class VRTASK_API UVRTPickupsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection)	override;
	
	UFUNCTION(BlueprintPure, Category = "VRTask|Pickups")
	bool FindPickupById(const FName& PickupId, FPickupGameData& OutPickupGameData) const;
	
private:
	
	TMap<FName, FPickupGameData> PickupsGameData;
	
	bool CollectPickupsDataFromDataTable(const UDataTable* InDataTable);
	void LogPickups() const;
};
