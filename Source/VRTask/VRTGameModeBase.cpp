// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTGameModeBase.h"
#include "VRTPawn.h"
#include "Components/VRTHealthComponent.h"

void AVRTGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AVRTPawn* PlayerPawn = Cast<AVRTPawn>(NewPlayer->GetPawn());
	UE_LOG(LogGameMode, Display, TEXT("Player '%s' logged in"), *NewPlayer->GetName());
}

void AVRTGameModeBase::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
	
}


