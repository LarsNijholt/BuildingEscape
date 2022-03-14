// No Copyright


#include "Grabber.h"
#include "Engine.h"
#include "GameFramework/PlayerController.h"

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

	FindPhysicsHandle();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent) PhysicsHandle->SetTargetLocation(GetPlayersReach());
}

void UGrabber::Grab()
{
	FHitResult Hit;
	FHitResult HitResult = Raycast();
	AActor* ActorHit = HitResult.GetActor();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	if (ActorHit)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetPlayersReach());
	}
}


void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent) PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::GetPlayersReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FVector UGrabber::GetPlayerWorldPosition() const
{
	//Get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) UE_LOG(LogTemp, Error, TEXT("%s has no Physicshandle attached"), *GetOwner()->GetName());
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found on %s"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

FHitResult UGrabber::Raycast()
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerWorldPosition(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	return Hit;
}
