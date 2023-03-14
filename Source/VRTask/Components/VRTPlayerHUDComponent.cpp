// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTPlayerHUDComponent.h"
#include "VRTask/UI/VRTWorldWidgetActor.h"

DECLARE_LOG_CATEGORY_CLASS(LogHUD, All, All);

UVRTPlayerHUDComponent::UVRTPlayerHUDComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;
}

void UVRTPlayerHUDComponent::ToggleMenuWidget(bool bRightHand)
{
	check(MainMenuWidgetActorClass);

	if (ActiveWidget != nullptr && ActiveWidget->IsA(MainMenuWidgetActorClass))
	{
		CloseActiveWidget();
		return;
	}
	
	SpawnWidget(MainMenuWidgetActorClass, bRightHand);
}

void UVRTPlayerHUDComponent::ToggleInventoryWidget(bool bRightHand)
{
	check(InventoryWidgetActorClass);
	
	if (ActiveWidget != nullptr && ActiveWidget->IsA(InventoryWidgetActorClass))
	{
		CloseActiveWidget();
		return;
	}

	SpawnWidget(InventoryWidgetActorClass, bRightHand);
}

void UVRTPlayerHUDComponent::CloseActiveWidget()
{
	if (ActiveWidget)
	{
		ActiveWidget->Destroy();
		ActiveWidget = nullptr;

		// Clearing all delegates here, since I don't see any point to left them
		OnTriggerPressed.Clear();
		OnNavigateRight.Clear();
		OnNavigateLeft.Clear();
	}
}

void UVRTPlayerHUDComponent::HUDNavigateLeft(bool bRightHand)
{
	if (!ActiveWidget && bActiveHandRight != bRightHand)
	{
		return;
	}

	if (OnNavigateLeft.IsBound())
	{
		OnNavigateLeft.Broadcast();
	}
}

void UVRTPlayerHUDComponent::HUDNavigateRight(bool bRightHand)
{
	if (!ActiveWidget && bActiveHandRight != bRightHand)
	{
		return;
	}

	if (OnNavigateRight.IsBound())
	{
		OnNavigateRight.Broadcast();
	}
}

void UVRTPlayerHUDComponent::HUDTriggerDown(bool bRightHand)
{
	if(!ActiveWidget && !OnTriggerPressed.IsBound())
	{
		return;
	}

	OnTriggerPressed.Broadcast(bRightHand);
}

void UVRTPlayerHUDComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UVRTPlayerHUDComponent::SpawnWidget(TSubclassOf<AVRTWorldWidgetActor> WidgetToSpawn, bool bRightHand)
{
	if (ActiveWidget)
	{
		UE_LOG(LogHUD, Display, TEXT("Failed to spawn widget since there is an active widget"));
		return;
	}

	ActiveWidget = GetWorld()->SpawnActor<AVRTWorldWidgetActor>(WidgetToSpawn);

	FWidgetActorInitializationParameters InitParams;
	bActiveHandRight = bRightHand;
	InitParams.bActiveHandRight = bActiveHandRight;
	
	if (ActiveWidget)
	{
		ActiveWidget->Initialize(InitParams);
	}
}
