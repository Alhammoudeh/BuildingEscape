// Copyright Alaa 2020

#include "OpenDoor.h"
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

	if (!OpenPlate) 
	{
		// ActorName has the open door component on it, but no pressure plate set.
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressure plate set."), *GetOwner()->GetName());
	}

	// Set Actor that opens door
	WhichActorOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
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
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("The value of Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);

	CurrentYaw = FMath::Lerp(CurrentYaw, YawAngle, DeltaTime * VelocityOfOpenDoor);
	FRotator OpenDoor(0.f, CurrentYaw, 0.f);

	// Set Actor Rotation
	GetOwner()->SetActorRotation(OpenDoor);

	//float CurrentYaw = GetOwner()->GetActorRotation().Yaw;

	//// FRotator OpenDoor
	//// Change Yaw value of OpenDoor

	//FRotator OpenDoor(0.f, 0.f, 0.f);
	//OpenDoor.Yaw = FMath::Lerp(CurrentYaw, YawAngle, 0.01f);
	//GetOwner()->SetActorRotation(OpenDoor);

	/*float DoorOpen = 90.f;
	FRotator CurrentRotation(GetOwner()->GetActorRotation());
	CurrentRotation.Add(CurrentRotation.Pitch, DoorOpen, CurrentRotation.Roll);
	GetOwner()->SetActorRotation(CurrentRotation);*/
}

void UOpenDoor::CloseDoor(float DeltaTime) 
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * VelocityOfCloseDoor);
	FRotator CloseDoor (0.f, CurrentYaw, 0.f);
	GetOwner()->SetActorRotation(CloseDoor);
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	// Find all overlapping actors.
	TArray<AActor*> OverlappingActors;

	OpenPlate->GetOverlappingActors(OUT OverlappingActors);

	// Add up their masses
	for (AActor* Actor: OverlappingActors) 
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

