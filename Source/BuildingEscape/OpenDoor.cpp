// Copyright Alaa 2020

#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Gameframework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	YawAngle += InitialYaw;

	IsPressurePlateSet();
	FindAudioComponent();
}

void UOpenDoor::IsPressurePlateSet() const
{
	if (!OpenPlate)
	{
		// ActorName has the open door component on it, but no pressure plate set.
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressure plate set."), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent) 
	{
		UE_LOG(LogTemp, Error, TEXT("Missing audio component for %s"), *GetOwner()->GetName());
	}

}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MassToOpenDoors)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else 
	{
		// if the door has been open longer than DoorClosedDelay seconds
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorClosedDelay)
		{
			CloseDoor(DeltaTime);
		}
	}

}

void UOpenDoor::OpenDoor(float DeltaTime) 
{
	CurrentYaw = FMath::Lerp(CurrentYaw, YawAngle, DeltaTime * VelocityOfOpenDoor);
	FRotator OpenDoor(0.f, CurrentYaw, 0.f);

	// Set Actor Rotation
	GetOwner()->SetActorRotation(OpenDoor);

	if (!AudioComponent) {return;}

	if (!OpenDoorSoundPlay) {
		AudioComponent->Play();
		CloseDoorSoundPlay = false;
		OpenDoorSoundPlay = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime) 
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * VelocityOfCloseDoor);
	FRotator CloseDoor (0.f, CurrentYaw, 0.f);
	GetOwner()->SetActorRotation(CloseDoor);

	if (!AudioComponent){ return;}

	if (!CloseDoorSoundPlay) {
		AudioComponent->Play();
		CloseDoorSoundPlay = true;
		OpenDoorSoundPlay = false;
	}
}
	

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	// Find all overlapping actors.
	TArray<AActor*> OverlappingActors;
	if (!OpenPlate) {return TotalMass;}
	OpenPlate->GetOverlappingActors(OUT OverlappingActors);

	// Add up their masses
	for (AActor* Actor: OverlappingActors) 
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

