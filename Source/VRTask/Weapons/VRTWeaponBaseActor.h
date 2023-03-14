// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRTask/VRTPickupActor.h"
#include "VRTWeaponBaseActor.generated.h"

class USceneComponent;
class UWidgetComponent;
class UVRTWeaponAmmoWidget;
class USoundBase;
class UHapticFeedbackEffect_Base;

UENUM(BlueprintType)
enum class EFiringMode : uint8
{
	Manual,
	Auto
};

/*
 * A base class for all shooting weapons
 */
UCLASS()
class VRTASK_API AVRTWeaponBaseActor : public AVRTPickupActor
{
	GENERATED_BODY()
	
public:	
	AVRTWeaponBaseActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* AmmoWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* MuzzleComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	FName MuzzleSocketName = "Muzzle";

	// How far the weapon shoots
	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	float MaxRange = 1500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	int32 MaxAmmoInMagazine = 30;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	float BaseDamage = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	float FireRange = 1000.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	USoundBase* EmptyFireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	TArray<EFiringMode> AvailableFiringMods;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	UHapticFeedbackEffect_Base* FireHapticEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon params")
	TSubclassOf<UDamageType> DamageType;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Game")
	void OnFire();

	// BEGIN: VRTInteractableInterface
	void ReloadPressed();
	virtual void ReloadPressed_Implementation() override;

	void TriggerPressed();
	virtual void TriggerPressed_Implementation() override;

	void TriggerReleased();
	virtual void TriggerReleased_Implementation() override;
	// END: VRTInteractableInterface

	UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void EndFire();

	void Reload();
	void Fire();
	void PlayFireEffect() const;
	void PlayImpactEffect(FVector EffectLocation) const;

private:

	FTransform GetMuzzleTransform() const;

	int32 AmmoInMagazine;

	UPROPERTY()
	UVRTWeaponAmmoWidget* WeaponAmmoWidget;

	float TimeBetweenShots = 0.1f;
	float LastFireTime;
	FTimerHandle TimeBetweenShotsHandler;
	bool bCanShoot = true;
	bool bCanReload = true;

	EFiringMode CurrentFiringMode;
	EControllerHand CurrentHand = EControllerHand::Left;

	APlayerController* GetPlayerController() const;

};
