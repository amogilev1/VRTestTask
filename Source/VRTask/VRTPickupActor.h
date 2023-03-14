// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/VRTInteractableInterface.h"
#include "VRTPickupActor.generated.h"

/**
 * A base class for all actors that can be added to the inventory
 */
class UVRTGrabComponent;
UCLASS()
class VRTASK_API AVRTPickupActor : public AActor,  public IVRTInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVRTPickupActor();

	UFUNCTION(BlueprintPure, Category="Pickups")
	FName GetPickupID() const
	{
		return PickupID;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	FName PickupID;
	
};
