// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;

	TargetYawToOpen += InitialYaw;

	Rotator = GetOwner()->GetActorRotation();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("The Actor: %s has no PressurePlate setted"), *GetOwner()->GetName());
	}

	GetAudioComponent();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActors() >= TargetMassToOpenDoor)
	{
		UpdateDoor(DeltaTime, TargetYawToOpen, DoorOpenSpeed);

		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if ((GetWorld()->GetTimeSeconds() - DoorLastOpened) >= DoorCloseDelay)
		{
			UpdateDoor(DeltaTime, InitialYaw, DoorCloseSpeed);
		}
	}
}

float UOpenDoor::GetCurrentYaw() const
{
	return GetOwner()->GetActorRotation().Yaw;
}

void UOpenDoor::UpdateDoor(const float& DeltaTime, const float& TargetYaw, const float& Speed)
{
	Rotator.Yaw = FMath::FInterpTo(GetCurrentYaw(), TargetYaw, DeltaTime, Speed);
	GetOwner()->SetActorRotation(Rotator);

	if (AudioComponent)
	{
		AudioComponent->Play();
	}
}

float UOpenDoor::GetTotalMassOfActors() const
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;

	if (PressurePlate)
	{
		PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	}

	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

void UOpenDoor::GetAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No AudioComponent was found on %s"), *GetOwner()->GetName());
	}
}
