// No Copyright


#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

	if (!PressurePlate) UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressureplate set"), *GetOwner()->GetName());
	FindAudioComponent();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent) UE_LOG(LogTemp, Error, TEXT("%s Missing audio component!"), *GetOwner()->GetName());
}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TotalMassOfActors() > MassThatOpens)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorClosedDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DeltaTime * OpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	CloseDoorSound = true;
	if (OpenDoorSound)
	{
		UE_LOG(LogTemp, Warning, TEXT("shot"));
		AudioComponent->Play();
		OpenDoorSound = false;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	OpenDoorSound = true;
	if (CloseDoorSound)
	{
		UE_LOG(LogTemp, Warning, TEXT("shot2"));
		AudioComponent->Play();
		CloseDoorSound = false;
	}
	CurrentYaw = FMath::Lerp(CurrentYaw, CloseYaw, DeltaTime * CloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += TotalMass + Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}
