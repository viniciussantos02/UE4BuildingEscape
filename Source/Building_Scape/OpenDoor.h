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
	void OpenDoor(const float& DeltaTime, const float& TargetYaw);
	void CloseDoor(const float& DeltaTime);
	float GetCurrentYaw() const;

private:
	FRotator Rotator;
	float InitialYaw;
	AActor* TargetActor;

	UPROPERTY(EditAnyWhere)
	float TargetYawToOpen = 90.f;

	UPROPERTY(EditAnyWhere)
	ATriggerVolume* PressurePlate;
};
