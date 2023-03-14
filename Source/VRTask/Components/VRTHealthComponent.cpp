// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTHealthComponent.h"

UVRTHealthComponent::UVRTHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UVRTHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (StartingHealth <= 0.0f)
	{
		StartingHealth = MaxHealth;
	}

	CurrentHealth = StartingHealth;
}


void UVRTHealthComponent::ApplyDamage(float Damage)
{
	if (bIsDead)
	{
		return;
	}
	
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, MinHealth, MaxHealth);

	if (CurrentHealth == MinHealth)
	{
		Death();
	}

	OnHealthUpdatedDelegate.Broadcast(CurrentHealth);
}

void UVRTHealthComponent::AddHealth(float InHealth)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + InHealth, MinHealth, MaxHealth);
}

float UVRTHealthComponent::GetCurrentHealthPercent()
{
	return MaxHealth / CurrentHealth;
}

void UVRTHealthComponent::Death()
{
	bIsDead = true;

	if (OnDeath.IsBound())
	{
		OnDeath.Broadcast(GetOwner());
	}
}
