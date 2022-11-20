// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveDoor.h"

AInteractiveDoor::AInteractiveDoor()
{
	SetRootComponent(GetStaticMeshComponent());

	DoorStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameComponent"));
	DoorStaticMeshComponent->SetupAttachment(GetStaticMeshComponent());
	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->InitBoxExtent(FVector(150.f, 100.f, 100.f));
	TriggerBox->SetupAttachment(DoorStaticMeshComponent);
	
	DoorMoveComponent = CreateDefaultSubobject<UDoorMoveComponent>(TEXT("DoorMoveComponent"));

	LockedMaterial = CreateDefaultSubobject<UMaterial>(TEXT("LockedMaterial"));
	UnlockedMaterial = CreateDefaultSubobject<UMaterial>(TEXT("UnlockedMaterial"));
	DoorStaticMeshComponent->SetMaterial(1, LockedMaterial);
}

void AInteractiveDoor::SwapDoorLatchState() const
{
	if (DoorMoveComponent->LatchState == ELatchState::ELS_Locked)
	{
		DoorMoveComponent->LatchState = ELatchState::ELS_Unlocked;
		DoorStaticMeshComponent->SetMaterial(1, UnlockedMaterial);
	}
	else
	{
		DoorMoveComponent->LatchState = ELatchState::ELS_Locked;
		DoorStaticMeshComponent->SetMaterial(1, LockedMaterial);
	}
}
