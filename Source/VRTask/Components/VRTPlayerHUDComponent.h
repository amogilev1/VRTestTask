// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VRTPlayerHUDComponent.generated.h"

class AVRTWorldWidgetActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNavigateLeftDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNavigateRightDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTriggerPressedDelegate, bool, bRightHand);

/*
 * Actor component that deal with controllable widgets that may be spawned in the world
 * Contains a logic of toggling MainMenu and InventoryMenu (may be extended)
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRTASK_API UVRTPlayerHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVRTPlayerHUDComponent();

	UFUNCTION(BlueprintCallable, Category = "GAME | UI")
	void ToggleMenuWidget(bool bRightHand);

	UFUNCTION(BlueprintCallable, Category = "GAME | UI")
	void ToggleInventoryWidget(bool bRightHand);

	UFUNCTION(BlueprintCallable, Category = "GAME | UI")
	void CloseActiveWidget();

	UFUNCTION(BlueprintCallable, Category = "GAME | UI")
	void HUDNavigateLeft(bool bRightHand);

	UFUNCTION(BlueprintCallable, Category = "GAME | UI")
	void HUDNavigateRight(bool bRightHand);

	UFUNCTION(BlueprintCallable, Category = "GAME | UI")
	void HUDTriggerDown(bool bRightHand);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "GAME | UI")
	TSubclassOf<AVRTWorldWidgetActor> MainMenuWidgetActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "GAME | UI")
	TSubclassOf<AVRTWorldWidgetActor> InventoryWidgetActorClass;

	UPROPERTY(BlueprintAssignable, Category = "GAME | UI")
	FNavigateLeftDelegate OnNavigateLeft;
	
	UPROPERTY(BlueprintAssignable, Category = "GAME | UI")
	FNavigateRightDelegate OnNavigateRight;

	UPROPERTY(BlueprintAssignable, Category = "GAME | UI")
	FTriggerPressedDelegate OnTriggerPressed;

private:

	UPROPERTY()
	AVRTWorldWidgetActor* ActiveWidget;

	void SpawnWidget(TSubclassOf<AVRTWorldWidgetActor> WidgetToSpawn, bool bRightHand);

	bool bActiveHandRight;
	
};
