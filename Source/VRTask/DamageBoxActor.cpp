// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageBoxActor.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ADamageBoxActor::ADamageBoxActor()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
}

void ADamageBoxActor::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADamageBoxActor::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ADamageBoxActor::OnBoxEndOverlap);

	GetWorld()->GetTimerManager().SetTimer(ApplyDamageTimerHandle, this, &ADamageBoxActor::ApplyDamage, DamageRate, true);
}

void ADamageBoxActor::ApplyDamage()
{
	for (int32 i = 0; i < ActorsToDamage.Num(); i++)
	{
		UGameplayStatics::ApplyDamage(ActorsToDamage[i], BaseDamage, nullptr, this, DamageType);
	}
}

void ADamageBoxActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
											UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
											const FHitResult& SweepResult)
{
	ActorsToDamage.AddUnique(OtherActor);
}

void ADamageBoxActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
										UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ActorsToDamage.Remove(OtherActor);
}
