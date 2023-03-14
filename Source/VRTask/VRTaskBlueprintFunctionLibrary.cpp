// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTaskBlueprintFunctionLibrary.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Engine/DebugCameraController.h"

APlayerController* UVRTaskBlueprintFunctionLibrary::GetLocalPlayerController(const UObject* WorldContextObject)
{
	check(WorldContextObject);

	const UWorld* World = LIKELY(GEngine) ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull) : nullptr;
	if (UNLIKELY(World == nullptr))
	{
		return nullptr;
	}

	APlayerController* PC = nullptr;

	// Get first local PlayerController
	for( FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator )
	{
		APlayerController* Controller = Iterator->Get();
		if (Controller && Controller->IsLocalController())
		{
			PC = Controller;
			break;
		}
	}
	
	if (PC == nullptr)
	{
		return nullptr;
	}
	
#if WITH_EDITOR
	// In the editor, we could be simulating and using ADebugCameraController instead
	if (PC->IsA<ADebugCameraController>())
	{
		return nullptr;
	}
#endif

	return PC;
}

bool UVRTaskBlueprintFunctionLibrary::PlayHapticEffect(const UObject* WorldContextObject, UHapticFeedbackEffect_Base* InHapticEffect,
	const EControllerHand& Hand, const float Scale, bool bLoop)
{
	APlayerController* PC = GetLocalPlayerController(GetLocalPlayerController(WorldContextObject));

	if (!PC)
	{
		return false;
	}

	PC->PlayHapticEffect(InHapticEffect, Hand, Scale, bLoop);
	return true;
}
