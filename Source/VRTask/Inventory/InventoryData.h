#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventoryData.generated.h"

class AVRTPickupActor;

/*
 * This file contains structs utilizing by the inventory system
 */


// Represents the data of the given Item that can be used in UI and in spawning actors
USTRUCT(BlueprintType)
struct FPickupGameData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemData")
	UTexture2D* ItemThumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemData")
	TSubclassOf<AVRTPickupActor> PickupActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemData")
	bool bIsStackable;
};

USTRUCT(BlueprintType)
struct FPickupData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "ItemData")
	FName ItemID;

	UPROPERTY(EditDefaultsOnly, Category = "ItemData")
	FPickupGameData PickupGameData;
};

// Represents a slot in the inventory
USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "ItemData")
	FName ItemID;

	UPROPERTY(BlueprintReadOnly, Category = "ItemData")
	int32 ItemCount = 1;

	bool operator == (const FInventorySlot& Slot) const
	{
		return Slot.ItemID == ItemID;
	}
};