// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VRTGameModeBase.generated.h"

/**
 * GameModeBase class for VRTask project
 */
UCLASS()
class VRTASK_API AVRTGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void RestartPlayer(AController* NewPlayer) override;
};
