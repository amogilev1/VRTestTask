// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTWeaponBaseActor.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "VRTWeaponAmmoWidget.h"
#include "Kismet/GameplayStatics.h"
#include "VRTask/VRTaskBlueprintFunctionLibrary.h"

DECLARE_LOG_CATEGORY_CLASS(LogWeapon, All, All);

AVRTWeaponBaseActor::AVRTWeaponBaseActor()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	SetRootComponent(WeaponMeshComponent);
	
	AmmoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("AmmoWidgetComponent");
	AmmoWidgetComponent->SetupAttachment(RootComponent);
}

void AVRTWeaponBaseActor::BeginPlay()
{
	Super::BeginPlay();

	if (AvailableFiringMods.Num() == 0)
	{
		// Let it be auto by default
		CurrentFiringMode = EFiringMode::Auto;
	}
	else
	{
		CurrentFiringMode = AvailableFiringMods[0];
	}
	
	AmmoInMagazine = MaxAmmoInMagazine;

	WeaponAmmoWidget = Cast<UVRTWeaponAmmoWidget>(AmmoWidgetComponent->GetWidget());

	if (WeaponAmmoWidget)
	{
		WeaponAmmoWidget->UpdateAmmo(AmmoInMagazine);
	}
}

void AVRTWeaponBaseActor::ReloadPressed_Implementation()
{
	IVRTInteractableInterface::ReloadPressed_Implementation();

	Reload();
}

void AVRTWeaponBaseActor::TriggerPressed_Implementation()
{
	IVRTInteractableInterface::TriggerPressed_Implementation();

	StartFire();
}

void AVRTWeaponBaseActor::TriggerReleased_Implementation()
{
	IVRTInteractableInterface::TriggerReleased_Implementation();

	EndFire();
}

void AVRTWeaponBaseActor::StartFire()
{
	if (AmmoInMagazine == 0)
	{
		return;
	}

	switch (CurrentFiringMode)
	{
		case EFiringMode::Manual:
			Fire();
			break;
		case EFiringMode::Auto:
			GetWorldTimerManager().SetTimer(TimeBetweenShotsHandler, this, &AVRTWeaponBaseActor::Fire, TimeBetweenShots, true);
			break;	
	}
}

void AVRTWeaponBaseActor::EndFire()
{
	GetWorldTimerManager().ClearTimer(TimeBetweenShotsHandler);
}

void AVRTWeaponBaseActor::Reload()
{
	if (AmmoInMagazine == MaxAmmoInMagazine)
	{
		return;
	}
	
	AmmoInMagazine = MaxAmmoInMagazine;
	if (WeaponAmmoWidget)
	{
		WeaponAmmoWidget->UpdateAmmo(AmmoInMagazine);
	}
}

void AVRTWeaponBaseActor::PlayFireEffect() const
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, WeaponMeshComponent, MuzzleSocketName);
	}

	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());
	}
}

void AVRTWeaponBaseActor::PlayImpactEffect(FVector EffectLocation) const
{
	if (ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, EffectLocation);
	}
}

FTransform AVRTWeaponBaseActor::GetMuzzleTransform() const
{
	return WeaponMeshComponent->GetSocketTransform(MuzzleSocketName);
}

APlayerController* AVRTWeaponBaseActor::GetPlayerController() const
{
	return UVRTaskBlueprintFunctionLibrary::GetLocalPlayerController(this);
}

void AVRTWeaponBaseActor::Fire()
{
	if (AmmoInMagazine == 0)
	{
		EndFire();
		
		if (EmptyFireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, GetActorLocation());
		}

		return;
	}

	OnFire();
	AmmoInMagazine -=  1;

	if (WeaponAmmoWidget)
	{
		WeaponAmmoWidget->UpdateAmmo(AmmoInMagazine);
	}
	
	PlayFireEffect();
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;

	const FTransform MuzzleTransform = GetMuzzleTransform();

	const FVector TraceStart = MuzzleTransform.GetLocation();
	const FVector ShootingDirection = MuzzleTransform.GetRotation().Vector();
	const FVector TraceEnd = TraceStart + ShootingDirection * MaxRange;

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		// add damage
		AActor* HitActor = Hit.GetActor();

		float ActualDamage = BaseDamage;

		UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShootingDirection, Hit, GetPlayerController(), this, DamageType);

		PlayImpactEffect(Hit.ImpactPoint);
	}
}
