// Fill out your copyright notice in the Description page of Project Settings.

#include "Ball.h"
#include "BallProjectileMovementComponent.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MovementComponent = CreateDefaultSubobject<UBallProjectileMovementComponent>(TEXT("BallProjectileMovementComponent"));
	
	RootComponent = StaticMeshComponent;
}

void ABall::Hit(const FVector& InTargetLocation, const EShotType InShotType)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 3.f, FColor::Green, InTargetLocation.ToString());
	
	if (MovementComponent != nullptr)
	{
		switch (InShotType)
		{
		case EShotType::FLAT:
			MovementComponent->DoFlatShot(InTargetLocation);
			break;
			
		case EShotType::TOPSPIN:
			MovementComponent->DoTopSpinShot(InTargetLocation);
			break;
			
		case EShotType::SLICE:
			MovementComponent->DoSliceShot(InTargetLocation);
			break;
		default:
			break;
		}
	}
}

