// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageBoxActor.generated.h"

class UBoxComponent;

/*
 * A simple box that hurt any actor that stand in it
 */
UCLASS()
class VRTASK_API ADamageBoxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADamageBoxActor();

protected:
	virtual void BeginPlay() override;
	void ApplyDamage();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float BaseDamage = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float DamageRate = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;
	
private:

	FTimerHandle ApplyDamageTimerHandle;

	UPROPERTY()
	TArray<AActor*> ActorsToDamage;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
