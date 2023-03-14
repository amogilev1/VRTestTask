// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRTBaseEnemy.generated.h"

class UWidgetComponent;
class UVRTHealthComponent;

/**
 * Base enemy class which handle enemy appearance, health, and death
 */

UCLASS()
class VRTASK_API AVRTBaseEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	AVRTBaseEnemy();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	UVRTHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* HealthWidgetComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	UParticleSystem* DeathEmitter;

public:
	
	virtual void Tick(float DeltaTime) override;
	
private:

	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION()
	void HandleDeath(AActor* DeadActor);

	void RotateHealthWidget() const;

};
