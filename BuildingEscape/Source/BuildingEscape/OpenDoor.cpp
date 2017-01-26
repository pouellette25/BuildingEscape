// Copyright LP Creative Ventures LLC

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// get the owning actor
	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// poll the trigger volume 
	if (GetTotalMassOfActorsOnPlate() > 30.0f) // TODO make threshold a parameter in editor
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// check if it's time to close the door
	if ((GetWorld()->GetTimeSeconds() - LastDoorOpenTime) > DoorCloseDelay)
	{
		CloseDoor();
		LastDoorOpenTime = 0.0f;
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;

	TArray<AActor*> OverlappingActors;

	/// find all the overlapping actors
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	/// iterate through them adding their masses
	for (const auto* Actor : OverlappingActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Name: %s"), *Actor->GetName());
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

