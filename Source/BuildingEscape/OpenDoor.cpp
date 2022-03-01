// No Copyright


#include "OpenDoor.h"
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

	float MyFloat = 90.f;
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	FRotator OpenDoor(0.f, 0.f, 0.f);
	GetOwner()->SetActorRotation(OpenDoor);
}


void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Warning, TEXT("%f"), GetOwner()->GetActorRotation().Yaw);
}

