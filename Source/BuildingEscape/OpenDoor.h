// No Copyright

#pragma once

#include "Engine.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
		float TargetYaw;
	UPROPERTY(EditAnywhere)
		float CloseYaw;
	UPROPERTY(EditAnywhere)
		float OpenSpeed;
	UPROPERTY(EditAnywhere)
		float CloseSpeed;
	float DoorLastOpened;
	bool OpenDoorSound = false;
	bool CloseDoorSound = false;

	UPROPERTY(EditAnywhere);
	float DoorClosedDelay = 0.5f;
	UPROPERTY(EditAnywhere)
		float MassThatOpens = 50.f;

	UAudioComponent* AudioComponent = nullptr;
	void FindAudioComponent();
	float InitialYaw;
	float CurrentYaw;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
		AActor* ActorThatOpens;
};
