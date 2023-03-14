// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRTPawn.generated.h"

class UVRTGrabComponent;
class UMotionControllerComponent;
class UVRTPlayerHUDComponent;
class UVRTInventoryComponent;
class UVRTHealthComponent;
class UCameraComponent;
class UBoxComponent;
class UVRTGameInstance;

UCLASS()
class VRTASK_API AVRTPawn : public APawn
{
	GENERATED_BODY()

public:
	AVRTPawn();
	
	UVRTHealthComponent* GetHealthComponent() const
	{
		return HealthComponent;
	}

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultRootComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UMotionControllerComponent* MotionControllerLeft;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UMotionControllerComponent* MotionControllerRight;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UVRTHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UVRTInventoryComponent* InventoryComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UVRTPlayerHUDComponent* HUDComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BackpackComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* MotionControllerLeftCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* MotionControllerRightCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Feedback")
	UHapticFeedbackEffect_Base* BackpackHapticEffect;

	UPROPERTY(BlueprintReadWrite, Category = "Game")
	UVRTGrabComponent* HeldComponentLeft;

	UPROPERTY(BlueprintReadWrite, Category = "Game")
	UVRTGrabComponent* HeldComponentRight;

	// Add held pickup that overlap with the backpack to the inventory (if possible) 
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TryAddHeldPickupToInventory();

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void TryNavigateHUDLeftWithLeftHand();
	void TryNavigateHUDRightWithLeftHand();

	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void HandleDeath(AActor* DeadActor);

	UFUNCTION()
	void OnBackPackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							const FHitResult& SweepResult);

	UFUNCTION()
	void OnBackPackEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintReadOnly, Category = "Game", meta=(AllowPrivateAccess))
	UVRTGrabComponent* HeldComponentNearBackpack;

	UFUNCTION()
	void OnReloadRightPressed();
	void OnReloadLeftPressed();

	void DropInventory();

	UVRTGameInstance* GetGameInstance();

	UPROPERTY()
	UVRTGameInstance* GameInstance;
};
