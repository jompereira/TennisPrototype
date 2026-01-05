// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Ball.h"
#include "TennisUtilitiesFunctionLibrary.h"
#include "Components/SphereComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RadiusSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RadiusSphereComponent"));
	RadiusSphereComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLastMovementInputVector().IsZero())
	{
		static const FRotator TargetIdleRotation = FRotator(0.f, 180.f, 0.f);
		SetActorRotation(FMath::RInterpTo(GetActorRotation(),
											TargetIdleRotation,
											DeltaTime,
											7.5f));
	}

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, -1.f, FColor::Blue, FString::Printf(TEXT("%d"), GetMesh()->IsPlayingRootMotion())); 
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::DoFlatShot()
{
}

void APlayerCharacter::DoTopSpinShot()
{
}

void APlayerCharacter::MoveTargetActor(const float InX, const float InY, const float InSpeedMultiplier)
{
	if (TargetActor)
	{
		TargetActor->AddActorLocalOffset(FVector(InX * InSpeedMultiplier, InY * InSpeedMultiplier, 0.0f), true);
	}
}

void APlayerCharacter::SetTargetActorLocation(const FVector& InLocation)
{
	if (TargetActor)
	{
		TargetActor->SetActorLocation(InLocation);
	}
}

