// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_SCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float GetCurrentYaw() const;
	void UpdateDoor(const float& DeltaTime, const float& TargetYaw, const float& Speed);

private:
	FRotator Rotator;
	float InitialYaw;
	AActor* TargetActor;
	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnyWhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnyWhere)
	float TargetYawToOpen = 90.f;

	UPROPERTY(EditAnyWhere)
	float DoorCloseDelay = 1.f;

	UPROPERTY(EditAnyWhere)
	float DoorOpenSpeed = 2.f;

	UPROPERTY(EditAnyWhere)
	float DoorCloseSpeed = 2.f;
};
