// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VRTaskBlueprintFunctionLibrary.generated.h"

class UHapticFeedbackEffect_Base;

/**
 * 
 */
UCLASS()
class VRTASK_API UVRTaskBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "VRTask|Utils|Player", meta = (WorldContext = "WorldContextObject"))
	static APlayerController* GetLocalPlayerController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "VRTask|Utils|Player", meta = (WorldContext = "WorldContextObject"))
	static bool PlayHapticEffect(const UObject* WorldContextObject, UHapticFeedbackEffect_Base* InHapticEffect, const EControllerHand& Hand, const float Scale, bool bLoop);
};
