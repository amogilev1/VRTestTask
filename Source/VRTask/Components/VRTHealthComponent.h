// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VRTHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDamageReceivedDelegate, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthUpdatedDelegate, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeathDelegate, class AActor*, KilledActor);

/*
 * Actor component that deal managing actor's health update and death
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRTASK_API UVRTHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UVRTHealthComponent();

protected:
	
	virtual void BeginPlay() override;

	// How much health we start with. Negative or 0 values will default to MaxHealth.
    // This can be used in cases where we want to spawn objects partially damaged already
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health Component")
    float StartingHealth = -1.0f;
    	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.1f), Category = "Health Component")
    float MaxHealth = 100.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.0f), Category = "Health Component")
    float MinHealth = 0.0f;


public:	
	
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetCurrentHealth() const
	{
		return CurrentHealth;
	}

	UFUNCTION(BlueprintCallable, Category="Health")
	void ApplyDamage(float Damage);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float InHealth);

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetCurrentHealthPercent();

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FHealthUpdatedDelegate OnHealthUpdatedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FDamageReceivedDelegate OnReceiveDamage;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FDeathDelegate OnDeath;

private:
	float CurrentHealth;
	void Death();
	bool bIsDead = false;
};
