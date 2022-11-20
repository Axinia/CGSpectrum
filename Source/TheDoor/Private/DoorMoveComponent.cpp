// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorMoveComponent.h"

#include "InteractiveDoor.h"
#include "WarriorCharacter.h"

// Sets default values for this component's properties
UDoorMoveComponent::UDoorMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UDoorMoveComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner())
	{
		TriggerBox = GetOwner()->FindComponentByClass<UBoxComponent>();
		TObjectPtr<AInteractiveDoor> Door = Cast<AInteractiveDoor>(GetOwner());
		InitialRotation = Door->GetDoorMesh()->GetRelativeRotation();
		CurrentRotation = InitialRotation;
		DesiredRotation = FRotator(InitialRotation.Pitch, InitialRotation.Yaw + GradesToRotate, InitialRotation.Roll);
	}
	// ...
	
}


// Called every frame
void UDoorMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AutomateDoor(DeltaTime);
	// ...
}

void UDoorMoveComponent::AutomateDoor(float DeltaTime)
{
	if (TriggerBox && GetWorld() && GetWorld()->GetFirstLocalPlayerFromController())
	{
		// Get door movement variables ready
		APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
		const float TimeRatio = FMath::Clamp(CurrentTimeToRotate / TimeToRotate, 0.0f, 1.0f);
		const float RotationAlpha = RotationCurve.GetRichCurveConst()->Eval(TimeRatio);
		const EDoorState CurrentDoorState = DoorState;

		if (Player && TriggerBox->IsOverlappingActor(Player))
		{
			TObjectPtr<AWarriorCharacter> Warrior = Cast<AWarriorCharacter>(Player);
			if (Warrior)
			{
				Warrior->SetOverlappingItem(GetOwner());
			}
		}
		else
		{
			TObjectPtr<AWarriorCharacter> Warrior = Cast<AWarriorCharacter>(Player);
			if (Warrior)
			{
				Warrior->SetOverlappingItem(nullptr);
			}
		}
		
		if (Player)
		{
			SetDesiredRotation(Player);
		}
		
		CurrentRotation = FMath::Lerp(InitialRotation, DesiredRotation, RotationAlpha);

		switch (CurrentDoorState)
		{
		case EDoorState::EDS_Closed:
			{
				if (Player && TriggerBox->IsOverlappingActor(Player) && (LatchState == ELatchState::ELS_Unlocked))
				{
					DoorState = EDoorState::EDS_Opening;
				}
				break;
			}
		case EDoorState::EDS_Opening:
			{
				if (LatchState == ELatchState::ELS_Unlocked)
				{
					if (CurrentTimeToRotate < TimeToRotate)
					{
						// Open door
						CurrentTimeToRotate = FMath::Clamp(CurrentTimeToRotate + DeltaTime, 0.f, TimeToRotate);
						TObjectPtr<AInteractiveDoor> Door = Cast<AInteractiveDoor>(GetOwner());
						Door->GetDoorMesh()->SetRelativeRotation(CurrentRotation);
					}
					else
					{
						DoorState = EDoorState::EDS_Opened;
					}
				}
				else
				{
					DoorState = EDoorState::EDS_Closing;
				}
				break;
			}
		case EDoorState::EDS_Opened:
			{
				if (Player && !TriggerBox->IsOverlappingActor(Player))
				{
					DoorState = EDoorState::EDS_Closing;
				}
				break;
			}
		case EDoorState::EDS_Closing:
			{
				if (CurrentTimeToRotate > 0.f)
				{
					// Open door
					CurrentTimeToRotate = FMath::Clamp(CurrentTimeToRotate - DeltaTime, 0.f, TimeToRotate);
					TObjectPtr<AInteractiveDoor> Door = Cast<AInteractiveDoor>(GetOwner());
					Door->GetDoorMesh()->SetRelativeRotation(CurrentRotation);
				}
				else
				{
					DoorState = EDoorState::EDS_Closed;
				}
				break;
			}

		default:
			UE_LOG(LogTemp, Warning, TEXT("Something went wrong with Switch Door State"))
		}
	}
}

void UDoorMoveComponent::SetDesiredRotation(const APawn* Player)
{
	if ( (Player) && (GetOwner()) && (DoorState == EDoorState::EDS_Closed))
	{
		FVector PlayerLocation = Player->GetActorLocation();
		FVector DoorLocation = GetOwner()->GetActorLocation();
		FVector DoorRightForward = GetOwner()->GetActorRightVector();
		FVector Direction = (DoorLocation - PlayerLocation);
		Direction.Normalize();
		const float Dot = FVector::DotProduct(DoorRightForward, Direction);

		// Make the door open in the direction the player is facing
		if (Dot > 0.f)
		{
			DesiredRotation = FRotator(InitialRotation.Pitch, InitialRotation.Yaw - GradesToRotate, InitialRotation.Roll);
		}
		else
		{
			DesiredRotation = FRotator(InitialRotation.Pitch, InitialRotation.Yaw + GradesToRotate, InitialRotation.Roll);
		}
	}
}


