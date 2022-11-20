// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "ItemOscillationMoveComponent.generated.h"

enum class EFloatingState
{
	EFS_Lowering,
	EFS_Uplifting,

	EFS_MAX
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEDOOR_API UItemOscillationMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemOscillationMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void PlayOscillationFloating(float DeltaTime);
	void PlayRotation(float DeltaTime);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, NoClear, Category = "Oscillation")
	float OscillationRange = 15.f;

	UPROPERTY(EditAnywhere, NoClear, Category = "Oscillation")
	float OscillationSpeed = 25.f;

	UPROPERTY(EditAnywhere, NoClear, Category = "Oscillation")
	float RotationSpeed = 50.f;

	FVector LowOscillation;
	FVector HighOscillation;
	TObjectPtr<UStaticMeshComponent> OwnerStaticMeshComponent = nullptr;
	TObjectPtr<USphereComponent> PickUpSphereComponent = nullptr;
	TObjectPtr<UWidgetComponent> PickUpWidgetComponent = nullptr;
	EFloatingState FloatingState = EFloatingState::EFS_Lowering;
};
