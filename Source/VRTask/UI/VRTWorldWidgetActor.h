// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRTWorldWidgetActor.generated.h"

/*
 * A base class for all widgets that spawned in the world and controlled by the pawn
 */

USTRUCT()
struct FWidgetActorInitializationParameters
{
	GENERATED_BODY()

	bool bActiveHandRight;
};

UCLASS()
class VRTASK_API AVRTWorldWidgetActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVRTWorldWidgetActor();

	void Initialize(const FWidgetActorInitializationParameters& InInitializationParams);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	bool bActiveHandRight;
};
