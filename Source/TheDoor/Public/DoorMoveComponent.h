// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/TriggerBox.h"
#include "DoorMoveComponent.generated.h"

UENUM(BlueprintType)
enum class EDoorState : uint8
{
	EDS_Opened		UMETA(DisplayName="Opened"),
	EDS_Opening		UMETA(DisplayName="Opening"),
	EDS_Closed		UMETA(DisplayName="Closed"),
	EDS_Closing		UMETA(DisplayName="Closing"),

	EDS_MAX			UMETA(DisplayName="MAX")
};

UENUM(BlueprintType)
enum class ELatchState : uint8
{
	ELS_Locked		UMETA(DisplayName="Locked"),
	ELS_Unlocked	UMETA(DisplayName="Unlocked"),

	ELS_MAX			UMETA(DisplayName="MAX")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEDOOR_API UDoorMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, Category = "Rotation")
	ELatchState LatchState = ELatchState::ELS_Locked;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Rotation")
	EDoorState DoorState = EDoorState::EDS_Closed;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	float GradesToRotate = 85.f;

	FRotator InitialRotation;
	FRotator CurrentRotation;
	FRotator DesiredRotation;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	float TimeToRotate = 1.5f;
	float CurrentTimeToRotate = 0.f;
	
	UPROPERTY(Transient)
	UBoxComponent* TriggerBox = nullptr;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	FRuntimeFloatCurve RotationCurve;
	
	void AutomateDoor(float DeltaTime);
	void SetDesiredRotation(const APawn* Player);
};
