// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "TheDoor/Public/DoorMoveComponent.h"
#include "Engine/StaticMeshActor.h"
#include "InteractiveDoor.generated.h"

/**
 * 
 */
UCLASS()
class THEDOOR_API AInteractiveDoor : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AInteractiveDoor();
	
	FORCEINLINE UBoxComponent* GetTriggerBox() const {return TriggerBox;}
	FORCEINLINE UStaticMeshComponent* GetDoorMesh() const {return DoorStaticMeshComponent;}
	void SwapDoorLatchState() const;
	
protected:
	UPROPERTY(EditAnywhere, NoClear, Category = Door)
	TObjectPtr<UStaticMeshComponent> DoorStaticMeshComponent;
	
	UPROPERTY(EditAnywhere, NoClear, Category = "Open/Close Door")
	TObjectPtr<UDoorMoveComponent> DoorMoveComponent;
	
	UPROPERTY(EditDefaultsOnly, NoClear, Category = "Rotation")
	TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterial> LockedMaterial;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterial> UnlockedMaterial;
};
