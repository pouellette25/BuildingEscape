// Copyright LP Creative Ventures LLC

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(EditAnywhere)
		float Reach;

private:
	UPhysicsHandleComponent* PhysicsHandle = nullptr;	
	UInputComponent* InputComponent = nullptr;

	void SetupInputComponent();
	void FindPhysicsHandleComponent();
	void Grab();
	void Release();

	// Returns current start of reach line
	FVector GetReachLineStart();

	// Returns current end of reach line
	FVector GetReachLineEnd();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
};
