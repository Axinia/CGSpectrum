// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemOscillationMoveComponent.h"

#include "VectorTypes.h"

// Sets default values for this component's properties
UItemOscillationMoveComponent::UItemOscillationMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UItemOscillationMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner()->FindComponentByClass<UStaticMeshComponent>())
	{
		OwnerStaticMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	}
	if (GetOwner()->FindComponentByClass<UWidgetComponent>())
	{
		PickUpWidgetComponent = GetOwner()->FindComponentByClass<UWidgetComponent>();
	}

	// Initialize Oscillation Edges
	LowOscillation = GetOwner()->GetActorLocation() - FVector(0.f, 0.f, OscillationRange);
	HighOscillation = GetOwner()->GetActorLocation() + FVector(0.f, 0.f, OscillationRange);
}

// Make the whole actor float, not just the mesh
void UItemOscillationMoveComponent::PlayOscillationFloating(float DeltaTime)
{
	const FVector CurrentLocation = GetOwner()->GetActorLocation();
	FVector NextLocation;
	
	if (FloatingState == EFloatingState::EFS_Lowering)
	{
		NextLocation = CurrentLocation - FVector(0.f, 0.f, OscillationSpeed * DeltaTime);
		GetOwner()->SetActorRelativeLocation(NextLocation);

		if (NextLocation.Z <= LowOscillation.Z)
		{
			FloatingState = EFloatingState::EFS_Uplifting;
		}
	}
	else if (FloatingState == EFloatingState::EFS_Uplifting)
	{
		NextLocation = CurrentLocation + FVector(0.f, 0.f, OscillationSpeed * DeltaTime);
		GetOwner()->SetActorRelativeLocation(NextLocation);

		if (NextLocation.Z >= HighOscillation.Z)
		{
			FloatingState = EFloatingState::EFS_Lowering;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UItemOscillationMoveComponent not working properly"))
	}
}

// Make the mesh rotate, NOT the whole actor
void UItemOscillationMoveComponent::PlayRotation(float DeltaTime)
{
	const FRotator CurrentRotation = OwnerStaticMeshComponent->GetRelativeRotation();
	const FRotator NextRotation = CurrentRotation + FRotator(0.f, RotationSpeed * DeltaTime,0.f);
	OwnerStaticMeshComponent->SetRelativeRotation(NextRotation);
}

// Called every frame
void UItemOscillationMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PlayOscillationFloating(DeltaTime);
	PlayRotation(DeltaTime);
}

