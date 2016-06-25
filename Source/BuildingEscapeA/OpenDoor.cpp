// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscapeA.h"
#include "OpenDoor.h"
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	// Find the owning Actor
	AActor* Owner = GetOwner();

	// Create a rotator
	FRotator NewRotation = FRotator(0.0f,60.0f,0.0f);
	
	//Set the door rotation
	Owner->SetActorRotation(NewRotation);
	
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the triger volume
	if (GetTotalMassOfActorsOnPlate() > 30.f) // TODO make into a parameter
	{

		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	
	//Check if it's time to close the door
	if (DoorCloseDelay < GetWorld()->GetTimeSeconds() - LastDoorOpenTime)
	{
		CloseDoor();

	}
	
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, 0.f, 0.f));
}

float UOpenDoor::GetTotalMassOfActorsOnPlate(){

	float TotalMass = 0.f;

	//find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// iterate through them adding their mass

	for (const auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName())
	}

	return TotalMass;
}