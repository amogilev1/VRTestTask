// Fill out your copyright notice in the Description page of Project Settings.


#include "VRTBaseEnemy.h"

#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "VRTask/VRTaskBlueprintFunctionLibrary.h"
#include "VRTask/Components/VRTHealthComponent.h"
#include "Kismet/KismetMathLibrary.h"

AVRTBaseEnemy::AVRTBaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UVRTHealthComponent>(TEXT("HealthComponent"));
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = StaticMeshComponent;

	HealthWidgetComponent->SetupAttachment(GetRootComponent());
}

void AVRTBaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnDeath.AddDynamic(this, &ThisClass::HandleDeath);
	OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeDamage);
	
}

void AVRTBaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateHealthWidget();
}

void AVRTBaseEnemy::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (!HealthComponent)
	{
		return;
	}

	HealthComponent->ApplyDamage(Damage);
}

void AVRTBaseEnemy::HandleDeath(AActor* DeadActor)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEmitter, GetActorLocation());
	Destroy();
}

void AVRTBaseEnemy::RotateHealthWidget() const
{
	const APlayerController* PlayerController = UVRTaskBlueprintFunctionLibrary::GetLocalPlayerController(this);

	if (!PlayerController)
	{
		return;
	}
	
	const FVector& PlayerCameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
	const FVector& HealthWidgetLocation = HealthWidgetComponent->GetComponentLocation();

	if (HealthWidgetComponent)
	{
		HealthWidgetComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(HealthWidgetLocation, PlayerCameraLocation));
	}
}

