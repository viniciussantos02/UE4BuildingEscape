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

	TargetActor = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("The Actor: %s has no PressurePlate setted"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(TargetActor))
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
}
