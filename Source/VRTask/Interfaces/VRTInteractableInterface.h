// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VRTInteractableInterface.generated.h"

/*
 * Interface that contains basic functions for interacting with held items
 */

UINTERFACE(MinimalAPI)
class UVRTInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class VRTASK_API IVRTInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TriggerPressed();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TriggerReleased();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReloadPressed();
};
