// Copyright Alaa 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float Deltatime);
	float TotalMassOfActors() const;

private:
	// declare member variables
	UPROPERTY(EditAnywhere)
	float YawAngle = 90.f;
	
	float InitialYaw;
	float CurrentYaw;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* OpenPlate;

	UPROPERTY(EditAnywhere)
	AActor* WhichActorOpens;

	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float DoorClosedDelay = 0.5f;

	UPROPERTY(EditAnywhere)
	float VelocityOfOpenDoor = 0.6f;

	UPROPERTY(EditAnywhere)
	float VelocityOfCloseDoor = 3.f;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoors = 50.f;
};
