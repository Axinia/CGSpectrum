// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorCharacter.h"

#include "InteractiveDoor.h"
#include "KingKey.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AWarriorCharacter::AWarriorCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bHasKey = false;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh()); 
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AWarriorCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWarriorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AWarriorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AWarriorCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWarriorCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AWarriorCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &AWarriorCharacter::Turn);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AWarriorCharacter::Jump);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &AWarriorCharacter::EquipItem);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AWarriorCharacter::Interact);
}

void AWarriorCharacter::SetOverlappingItem(AActor* Item)
{
	const TObjectPtr<AInteractiveDoor> Door = Cast<AInteractiveDoor>(Item);
	if (Door)
	{
		OverlapItem = Item;
	}

	const TObjectPtr<AKingKey> Key = Cast<AKingKey>(Item);
	if (Key)
	{
		OverlapItem = Item;
	}
}


void AWarriorCharacter::MoveForward(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.f))
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, AxisValue);
	}
}

void AWarriorCharacter::MoveRight(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.f))
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, AxisValue);
	}
}

void AWarriorCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AWarriorCharacter::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AWarriorCharacter::EquipItem()
{
	TObjectPtr<AKingKey> Key = nullptr;
	if (OverlapItem)
	{
		Key = Cast<AKingKey>(OverlapItem);
	}
	
	if (Key)
	{
		bHasKey = true;
		Key->Destroy();
	}
}

void AWarriorCharacter::Interact()
{
	TObjectPtr<AInteractiveDoor> Door = nullptr;
	if (OverlapItem)
	{
		Door = Cast<AInteractiveDoor>(OverlapItem);
	}
	
	if (Door && bHasKey)
	{
		Door->SwapDoorLatchState();
	}
}

