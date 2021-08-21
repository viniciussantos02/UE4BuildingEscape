// Fill out your copyright notice in the Description page of Project Settings.

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
	GetLineTraceEnd();

	if (PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

void UGrabber::GrabPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("PRESSED"));
	
	Hit = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent* TargetComponentToGrab = Hit.GetComponent();

	if (Hit.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(
			TargetComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
	}
}

void UGrabber::GrabReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("RELESSED"))
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::CheckPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
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
	GetLineTraceEnd();

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	FHitResult HitResul;

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResul,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	if (Hit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor hitted: %s"), *Hit.GetActor()->GetName())
	}

	return HitResul;
}

void UGrabber::GetLineTraceEnd()
{
	PlayerController->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * ReachArm;
}

