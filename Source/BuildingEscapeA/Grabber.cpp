// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscapeA.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent() {
	/// Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component!"), *GetOwner()->GetName())
	}
}
/// Look for attached input comonent
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {

		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName())
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed!"))

	///LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh in our case
	auto ActorHit = HitResult.GetActor();
	///If we hit something attach a physics handle
	if (ActorHit) {
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None/*no bones needed*/, ComponentToGrab->GetOwner()->GetActorLocation(), true /*allows rotation*/);
	}
}

void UGrabber::Release() {
		PhysicsHandle->ReleaseComponent();
}
 
// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		//move the object we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Ray-cast out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
		);


	/// See what we hit
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{

		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()))
	}

	return HitResult;
}

FVector UGrabber::GetReachLineStart() {
	// Get player  viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT	PlayerViewPointLocation, PlayerViewPointRotation);
	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() {
	// Get player  viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT	PlayerViewPointLocation, PlayerViewPointRotation);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}