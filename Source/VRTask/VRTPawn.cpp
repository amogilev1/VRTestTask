// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTPawn.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "VRTaskBlueprintFunctionLibrary.h"
#include "VRTGameInstance.h"
#include "VRTGameModeBase.h"
#include "VRTPickupActor.h"
#include "Components/VRTHealthComponent.h"
#include "Components/VRTPlayerHUDComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/VRTGrabComponent.h"
#include "Interfaces/VRTInteractableInterface.h"
#include "Inventory/VRTInventoryComponent.h"

AVRTPawn::AVRTPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	HealthComponent = CreateDefaultSubobject<UVRTHealthComponent>(TEXT("HealthComponent"));
	InventoryComponent = CreateDefaultSubobject<UVRTInventoryComponent>(TEXT("InventoryComponent"));
	HUDComponent = CreateDefaultSubobject<UVRTPlayerHUDComponent>(TEXT("HUD Component"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	BackpackComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Backpack"));
	MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerLeft"));
	MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerRight"));
	MotionControllerLeftCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftControllerCollision"));
	MotionControllerRightCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightControllerCollision"));

	RootComponent = DefaultRootComponent;
	CameraComponent->SetupAttachment(GetRootComponent());
	MotionControllerLeft->SetupAttachment(GetRootComponent());
	MotionControllerRight->SetupAttachment(GetRootComponent());
	MotionControllerLeftCollision->SetupAttachment(MotionControllerLeft);
	MotionControllerRightCollision->SetupAttachment(MotionControllerRight);
	BackpackComponent->SetupAttachment(CameraComponent);
}

void AVRTPawn::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AVRTPawn::OnTakeDamage);

	if(HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &ThisClass::HandleDeath);
	}

	if (BackpackComponent)
	{
		BackpackComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBackPackBeginOverlap);
		BackpackComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnBackPackEndOverlap);
	}

	// Trying to set the inventory from the game instance
	if (GetGameInstance()->GetPlayerInventory().Num() > 0)
	{
		InventoryComponent->SetInventory(GetGameInstance()->GetPlayerInventory());
	}
}

void AVRTPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EEndPlayReason::LevelTransition)
	{
		if (InventoryComponent)
		{
			for (const FInventorySlot& Slot : InventoryComponent->GetInventory())
			{
				GetGameInstance()->AddToPlayerInventory(Slot);
			}
		}
	}
}

void AVRTPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVRTPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction(FName("MenuNavigationLeftStickRight"), EInputEvent::IE_Pressed, this, &AVRTPawn::TryNavigateHUDRightWithLeftHand);
	PlayerInputComponent->BindAction(FName("MenuNavigationLeftStickLeft"), EInputEvent::IE_Pressed, this, &AVRTPawn::TryNavigateHUDLeftWithLeftHand);

	PlayerInputComponent->BindAction(FName("ReloadRight"), EInputEvent::IE_Pressed, this, &ThisClass::OnReloadRightPressed);
	PlayerInputComponent->BindAction(FName("ReloadLeft"), EInputEvent::IE_Pressed, this, &ThisClass::OnReloadLeftPressed);
}

void AVRTPawn::TryNavigateHUDLeftWithLeftHand()
{
	if (!HUDComponent)
	{
		return;
	}

	HUDComponent->HUDNavigateLeft(false);
}

void AVRTPawn::TryNavigateHUDRightWithLeftHand()
{
	if (!HUDComponent)
	{
		return;
	}

	HUDComponent->HUDNavigateRight(false);
}

void AVRTPawn::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
	AActor* DamageCauser)
{
	checkf(HealthComponent, TEXT("Attempted to take damage while HealthComponent is nullptr"));

	HealthComponent->ApplyDamage(Damage);
}

void AVRTPawn::HandleDeath(AActor* DeadActor)
{
	DropInventory();
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
	AController* ControllerRef = GetController();
	Destroy();
	
	if (const UWorld* World = GetWorld())
	{
		if (AVRTGameModeBase* GameMode = Cast<AVRTGameModeBase>(World->GetAuthGameMode()))
		{
			GameMode->RestartPlayer(ControllerRef);
		}
	}
}

void AVRTPawn::OnBackPackBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp == MotionControllerLeftCollision)
	{
		UVRTaskBlueprintFunctionLibrary::PlayHapticEffect(this, BackpackHapticEffect, EControllerHand::Left, 1.f, false);

		HeldComponentNearBackpack = HeldComponentLeft;
	}

	if (OtherComp == MotionControllerRightCollision)
	{
		UVRTaskBlueprintFunctionLibrary::PlayHapticEffect(this, BackpackHapticEffect, EControllerHand::Right, 1.f, false);

		HeldComponentNearBackpack = HeldComponentRight;
	}
}

void AVRTPawn::OnBackPackEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	HeldComponentNearBackpack = nullptr;
}

void AVRTPawn::OnReloadRightPressed()
{
	if (!HeldComponentRight)
	{
		return;
	}

	const IVRTInteractableInterface* InteractableInterface = Cast<IVRTInteractableInterface>(HeldComponentRight->GetOwner());

	if (InteractableInterface)
	{
		InteractableInterface->Execute_ReloadPressed(HeldComponentRight->GetOwner());
	}
}

void AVRTPawn::OnReloadLeftPressed()
{
	if (!HeldComponentLeft)
	{
		return;
	}

	const IVRTInteractableInterface* InteractableInterface = Cast<IVRTInteractableInterface>(HeldComponentLeft->GetOwner());

	if (InteractableInterface)
	{
		InteractableInterface->Execute_ReloadPressed(HeldComponentLeft->GetOwner());
	}
}

void AVRTPawn::DropInventory()
{
	if (!InventoryComponent)
	{
		return;
	}

	InventoryComponent->DropInventory();
	GetGameInstance()->ClearPlayerInventory();
}

UVRTGameInstance* AVRTPawn::GetGameInstance()
{
	if (!GameInstance)
	{
		GameInstance = GetWorld()->GetGameInstance<UVRTGameInstance>();
	}
	check(GameInstance);
	
	return GameInstance;
}


bool AVRTPawn::TryAddHeldPickupToInventory()
{
	if (!HeldComponentNearBackpack)
	{
		return false;
	}
	
	AVRTPickupActor* HeldPickup = Cast<AVRTPickupActor>(HeldComponentNearBackpack->GetOwner());

	if (!HeldPickup)
	{
		return false;
	}
			
	check(InventoryComponent);
	if (InventoryComponent->AddToInventory(HeldPickup))
	{
		if (HeldComponentLeft == HeldComponentNearBackpack)
		{
			HeldComponentLeft = nullptr;
		}

		if (HeldComponentRight == HeldComponentNearBackpack)
		{
			HeldComponentRight = nullptr;
		}

		return true;
	}

	return false;
}
