// Copyright LP Creative Ventures LLC

#include "BuildingEscape.h"
#include "Grabber.h"

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

	FindPhysicsHandleComponent();

	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		/// bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find Input Component on %s"), *GetOwner()->GetName());
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	/// look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find PhysicsHandleComponent on %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"));

	/// try and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	/// if we hit something, then attach a physics handle
	if (ActorHit)
	{
		/// attach physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			FRotator(0)
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"));

	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	FRotator PlayerRotation;
	FVector PlayerLocation;

	// get the player viewpoint this tick
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);

	FVector LineTraceEnd = PlayerLocation + (PlayerRotation.Vector() * Reach);

	/// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		/// move the object that we're holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
		
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FRotator PlayerRotation;
	FVector PlayerLocation;

	// get the player viewpoint this tick
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);

	FVector LineTraceEnd = PlayerLocation + (PlayerRotation.Vector() * Reach);

	/// setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;

	// ray cast out to reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/// see what we hit
	AActor* HitActor = Hit.GetActor();
	if (HitActor)
	{
		/// log what we hit
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
	}

	return Hit;
}

