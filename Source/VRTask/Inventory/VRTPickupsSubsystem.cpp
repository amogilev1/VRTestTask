// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTPickupsSubsystem.h"

#include "VRTPickupsSettings.h"

DECLARE_LOG_CATEGORY_CLASS(LogPickupSubsystem, All, All);


void UVRTPickupsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UVRTPickupsSettings* Settings = NewObject<UVRTPickupsSettings>(UVRTPickupsSettings::StaticClass());
	checkf(Settings, TEXT("Tried to initialize pickups subsystem while picups settings is invalid"));
	
	if (!CollectPickupsDataFromDataTable(LoadObject<UDataTable>(nullptr, *Settings->SourceDataTable)))
	{
		UE_LOG(LogPickupSubsystem, Error, TEXT("UVRTPickupsSubsystem initialization failed!"));
	}

	bInitialized = true;
	UE_LOG(LogPickupSubsystem, Display, TEXT("UVRTPickupsSubsystem initialized"));
	LogPickups();
}

bool UVRTPickupsSubsystem::FindPickupById(const FName& PickupId, FPickupGameData& OutPickupGameData) const
{
	if (!bInitialized)
	{
		return false;
	}
	
	const FPickupGameData* PickupGameData = PickupsGameData.Find(PickupId);

	if (!PickupGameData)
	{
		UE_LOG(LogPickupSubsystem, Error, TEXT("Failed to find pickup with specified ID: %s"), *PickupId.ToString());
		return false;
	}

	OutPickupGameData.ItemThumbnail = PickupGameData->ItemThumbnail;
	OutPickupGameData.PickupActorClass = PickupGameData->PickupActorClass;
	OutPickupGameData.bIsStackable = PickupGameData->bIsStackable;
	
	return true;
}

bool UVRTPickupsSubsystem::CollectPickupsDataFromDataTable(const UDataTable* InDataTable)
{
	checkf(InDataTable, TEXT("Tried to collect pickups data while the source data table is nullptr"));
	
	TArray<FName> RowNames = InDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		const FString ContextString;
		const FPickupData* Row = InDataTable->FindRow<FPickupData>(RowName, ContextString);

		if (Row)
		{
			PickupsGameData.Add(Row->ItemID, Row->PickupGameData);
		}
	}

	return true;
}

void UVRTPickupsSubsystem::LogPickups() const
{
	UE_LOG(LogPickupSubsystem, Display, TEXT("Collected Pickups:"));
	for (const TPair<FName, FPickupGameData>& PickupData : PickupsGameData)
	{
		UE_LOG(LogPickupSubsystem, Display, TEXT("id: %s"), *PickupData.Key.ToString());
	}
}
