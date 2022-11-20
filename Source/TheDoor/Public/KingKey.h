// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemOscillationMoveComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMeshActor.h"
#include "KingKey.generated.h"

/**
 * 
 */
UCLASS()
class THEDOOR_API AKingKey : public AActor
{
	GENERATED_BODY()

public:
	AKingKey();
	void SetWidgetVisibility(bool bVisibility) const;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnPickUpSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);

	UFUNCTION()
	virtual void OnPickUpSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
		);
	
public:
	// called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Static Mesh Actor")
	TObjectPtr<UStaticMeshComponent> KeyStaticMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, NoClear, Category = "ParticleFX")
	TObjectPtr<UParticleSystem> SparkFX;
	
	TObjectPtr<UParticleSystemComponent> SparkParticleComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> PickUpSphereComponent;

	UPROPERTY(EditAnywhere, NoClear, Category = Oscillation)
	TObjectPtr<UItemOscillationMoveComponent> ItemOscillationMoveComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<UWidgetComponent> PickUpWidgetComponent;

	void OrientWidgetToPlayer() const;
};