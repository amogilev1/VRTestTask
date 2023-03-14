// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTWorldWidgetActor.h"

AVRTWorldWidgetActor::AVRTWorldWidgetActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AVRTWorldWidgetActor::Initialize(const FWidgetActorInitializationParameters& InInitializationParams)
{
	bActiveHandRight = InInitializationParams.bActiveHandRight;
}

void AVRTWorldWidgetActor::BeginPlay()
{
	Super::BeginPlay();
	
}
