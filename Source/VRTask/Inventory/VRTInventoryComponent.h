// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VRTask/Inventory/InventoryData.h"
#include "VRTInventoryComponent.generated.h"

class UVRTPickupsSubsystem;
class AVRTPickupActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAddDelegate, FName, ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemRemovedDelegate, FName, ItemID, AVRTPickupActor*, SpawnedActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRTASK_API UVRTInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVRTInventoryComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory")
	int32 MaxSlots;

public:

	UFUNCTION(BlueprintPure, Category="Inventory")
	TArray<FInventorySlot> GetInventory();

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddToInventory(AVRTPickupActor* PickupToAdd);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool GetPickupGameData(const FName& PickupID, FPickupGameData& OutPickupGameData) const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	AVRTPickupActor* TakePickupFromInventory(const FName& PickupID);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	int32 FindInventorySlotIndexByItemID(const FName& ItemID) const;
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveItemFromInventory(const FName& ItemID);
	
	void DropInventory();

	UPROPERTY(BlueprintAssignable)
	FOnItemAddDelegate OnItemAdded;

	UPROPERTY(BlueprintAssignable)
	FOnItemRemovedDelegate OnItemRemoved;

	void SetInventory(TArray<FInventorySlot> InInventory)
	{
		Inventory = InInventory;
	}

private:
	TArray<FInventorySlot> Inventory;

	UPROPERTY()
	UVRTPickupsSubsystem* PickupsSubsystem;

	AVRTPickupActor* SpawnItemInWorld(TSubclassOf<AVRTPickupActor> ActorClassToSpawn, const FVector& Location) const;
};
