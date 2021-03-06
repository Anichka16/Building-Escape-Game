// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPEA_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// How faro from the player can we reach in cm
	float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Ray-cast and grab what's in reach
	void Grab();
	// Called when Grab is released
	void Release();
	//Find attached physics handle
	void FindPhysicsHandleComponent();
	//Setup input component
	void SetupInputComponent();
	//return hit for first physics body reach
	const FHitResult GetFirstPhysicsBodyInReach();
	//Returns current start of reach line
	FVector GetReachLineStart();
	//Returns current end of reach line
	FVector GetReachLineEnd();
};
