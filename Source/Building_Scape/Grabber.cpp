// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Grabber.h"
#include "Kismet/GameplayStatics.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	CheckPhysicsHandle();

	SetupInputBinds();	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (PhysicsHandle)
	{
		if (PhysicsHandle->GetGrabbedComponent())
		{
			PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
		}
	}
}

void UGrabber::GrabPressed()
{
	Hit = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent* TargetComponentToGrab = Hit.GetComponent();

	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		if (PhysicsHandle)
		{
			PhysicsHandle->GrabComponentAtLocation(
				TargetComponentToGrab,
				NAME_None,
				GetLineTraceEnd()
			);
		}
	}
}

void UGrabber::GrabReleased()
{
	if (PhysicsHandle)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

void UGrabber::CheckPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandleComponent was found in the %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputBinds()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::GrabPressed);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::GrabReleased);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Input Component was found in the %s"), *GetOwner()->GetName());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	FHitResult HitResul;

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResul,
		PlayerController->GetPawn()->GetActorLocation(),
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return HitResul;
}

FVector UGrabber::GetLineTraceEnd() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	PlayerController->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * ReachArm;
}

