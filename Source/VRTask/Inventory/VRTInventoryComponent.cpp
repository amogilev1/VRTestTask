// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTInventoryComponent.h"

#include "VRTPickupsSubsystem.h"
#include "VRTask/VRTPickupActor.h"

UVRTInventoryComponent::UVRTInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UVRTInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	PickupsSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UVRTPickupsSubsystem>();
}

TArray<FInventorySlot> UVRTInventoryComponent::GetInventory()
{
	return Inventory;
}

bool UVRTInventoryComponent::AddToInventory(AVRTPickupActor* PickupToAdd)
{
	if (!PickupToAdd && Inventory.Num() >= MaxSlots)
	{
		return false;
	}

	const FName& PickupID = PickupToAdd->GetPickupID();
	FPickupGameData PickupGameData;

	if (!GetPickupGameData(PickupID, PickupGameData))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to add pickup with ID: '%s' to the inventory!"), *PickupID.ToString());
		return false;
	}

	// Removing actor from the world
	PickupToAdd->Destroy();

	if (OnItemAdded.IsBound())
	{
		OnItemAdded.Broadcast(PickupID);
	}
	
	// Checking if pickup we are trying to add already exists in the inventory
	for (FInventorySlot& InventorySlot : Inventory)
	{
		if (InventorySlot.ItemID == PickupID)
		{
			if (PickupGameData.bIsStackable)
			{
				InventorySlot.ItemCount++;
			
				return true;
			}
		}
	}

	// Didn't found existing item with specified ID or pickup is not stackable, so adding a new slot
	FInventorySlot NewSlot;
	NewSlot.ItemID = PickupID;
	Inventory.Emplace(NewSlot);
	
	return true;
}

bool UVRTInventoryComponent::GetPickupGameData(const FName& PickupID, FPickupGameData& OutPickupGameData) const
{
	checkf(PickupsSubsystem, TEXT("Attempted to GetPickupGameData while Pickups subsystem is nullptr"));

	if (!PickupsSubsystem->FindPickupById(PickupID, OutPickupGameData))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to add pickup with ID: '%s' to the inventory!"), *PickupID.ToString());
		return false;
	}

	return true;
}

AVRTPickupActor* UVRTInventoryComponent::TakePickupFromInventory(const FName& PickupID)
{
	if (!RemoveItemFromInventory(PickupID))
	{
		return nullptr;
	}

	FPickupGameData PickupGameData;

	if (!GetPickupGameData(PickupID, PickupGameData))
	{
		return nullptr;
	}
	

	// Spawning pickup in the world
	const TSubclassOf<AVRTPickupActor> ClassToSpawn = PickupGameData.PickupActorClass;
	AVRTPickupActor* SpawnedActor = SpawnItemInWorld(ClassToSpawn, GetOwner()->GetActorLocation());

	if (OnItemRemoved.IsBound())
	{
		OnItemRemoved.Broadcast(PickupID, SpawnedActor);
	}
	
	return SpawnedActor;
}


int32 UVRTInventoryComponent::FindInventorySlotIndexByItemID(const FName& ItemID) const
{
	for (int i = 0; i < Inventory.Num(); i++)
	{
		if (Inventory[i].ItemID == ItemID)
		{
			return i;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Failed to find pickup with specified id: '%s' in the inventory!"), *ItemID.ToString());
	return INDEX_NONE;
}

bool UVRTInventoryComponent::RemoveItemFromInventory(const FName& ItemID)
{
	const int32 IndexToRemove = FindInventorySlotIndexByItemID(ItemID);
	
	if (IndexToRemove == INDEX_NONE)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to remove item with specified id: '%s' from the inventory!"), *ItemID.ToString());
	}

	FPickupGameData PickupGameData;

	if (!GetPickupGameData(ItemID, PickupGameData))
	{
		return false;
	}

	if (PickupGameData.bIsStackable && Inventory[IndexToRemove].ItemCount > 1)
	{
		Inventory[IndexToRemove].ItemCount--;
		return true;
	}

	Inventory.RemoveAt(IndexToRemove);
	return true;
}

void UVRTInventoryComponent::DropInventory()
{
	for (const FInventorySlot& Slot : Inventory)
	{
		FPickupGameData PickupGameData;

		if (!GetPickupGameData(Slot.ItemID, PickupGameData))
		{
			continue;
		}

		for (int32 i = 0; i < Slot.ItemCount; ++i)
		{
			SpawnItemInWorld(PickupGameData.PickupActorClass, GetOwner()->GetActorLocation());
		}
	}

	Inventory.Empty();
}

AVRTPickupActor* UVRTInventoryComponent::SpawnItemInWorld(TSubclassOf<AVRTPickupActor> ActorClassToSpawn, const FVector& Location) const
{
	const FActorSpawnParameters SpawnParams;

	return Cast<AVRTPickupActor>(GetWorld()->SpawnActor(*ActorClassToSpawn, &Location, nullptr, SpawnParams));
}
