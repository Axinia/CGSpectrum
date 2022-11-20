// Fill out your copyright notice in the Description page of Project Settings.


#include "KingKey.h"

#include "WarriorCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AKingKey::AKingKey()
{
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(DefaultSceneComponent);
	
	KeyStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	KeyStaticMeshComponent->SetupAttachment(RootComponent);
	
	PickUpSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpSphere"));
	PickUpSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PickUpSphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	PickUpSphereComponent->SetupAttachment(RootComponent);

	PickUpWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidget"));
	PickUpWidgetComponent->SetupAttachment(PickUpSphereComponent);
	
	ItemOscillationMoveComponent = CreateDefaultSubobject<UItemOscillationMoveComponent>(TEXT("ItemOscillationMoveComponent"));
}

void AKingKey::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.Target = this;
	PrimaryActorTick.SetTickFunctionEnable(true);
	PrimaryActorTick.RegisterTickFunction(GetLevel());

	SetWidgetVisibility(false);
	if (PickUpSphereComponent)
	{
		PickUpSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AKingKey::OnPickUpSphereBeginOverlap);	
		PickUpSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AKingKey::OnPickUpSphereEndOverlap);	
	}
	
	if (SparkFX)
	{
		SparkParticleComponent = UGameplayStatics::SpawnEmitterAttached(
			SparkFX,
			KeyStaticMeshComponent,
			NAME_None,
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition
			);
	}
}

void AKingKey::OnPickUpSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TObjectPtr<AWarriorCharacter> Warrior = Cast<AWarriorCharacter>(OtherActor);
	if (Warrior)
	{
		Warrior->SetOverlappingItem(this);
	}
	SetWidgetVisibility(true);
}

void AKingKey::OnPickUpSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TObjectPtr<AWarriorCharacter> Warrior = Cast<AWarriorCharacter>(OtherActor);
	if (Warrior)
	{
		Warrior->SetOverlappingItem(nullptr);
	}
	SetWidgetVisibility(false);
}

void AKingKey::OrientWidgetToPlayer() const
{
	if (GetWorld()->GetFirstLocalPlayerFromController())
	{
		const TObjectPtr<APawn> Player = GetWorld()->GetFirstPlayerController()->GetPawn();	
		if (Player)
		{
			TObjectPtr<UCameraComponent> PlayerCamera = Player->FindComponentByClass<UCameraComponent>();
			if (PlayerCamera)
			{
				const FVector CameraPosition = PlayerCamera->GetComponentLocation();
				const FRotator WidgetRotation = FVector(
					FVector(CameraPosition.X, CameraPosition.Y, PickUpSphereComponent->GetComponentLocation().Z)
					- PickUpWidgetComponent->GetComponentLocation()
					).Rotation();

				// Orient the widget to look at Player's Camera
				PickUpWidgetComponent->SetWorldRotation(WidgetRotation);
			}
		}
	}
}

void AKingKey::SetWidgetVisibility (bool bVisibility) const
{
	if (PickUpWidgetComponent)
	{
		PickUpWidgetComponent->SetVisibility(bVisibility);
	}
}

void AKingKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	OrientWidgetToPlayer();
}
