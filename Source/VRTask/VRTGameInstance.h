// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Inventory/InventoryData.h"
#include "VRTGameInstance.generated.h"

class UVRTInventoryComponent;

UCLASS()
class VRTASK_API UVRTGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	void ClearPlayerInventory()
	{
		PlayerInventory.Empty();
	}

	void AddToPlayerInventory(const FInventorySlot& InSlotToadd)
	{
		PlayerInventory.Add(InSlotToadd);
	}

	UFUNCTION(BlueprintPure)
	TArray<FInventorySlot> GetPlayerInventory() const
	{
		return PlayerInventory;
	}
	
private:
	
	TArray<FInventorySlot> PlayerInventory;
};
