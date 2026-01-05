// Fill out your copyright notice in the Description page of Project Settings.


#include "BallThrowerMachine.h"
#include "Ball.h"
#include "BallProjectileMovementComponent.h"

static FAutoConsoleVariable BallThrowerShotType(
						TEXT("BallThrower.ShotType"),
						0,
						TEXT("0: Random \n 1: Flat \n 2: TopSpin"));

static FAutoConsoleVariable BallThrowerMode(
						TEXT("BallThrower.Mode"),
						0,
						TEXT("0: Random \n 1: Center"));

// Sets default values
ABallThrowerMachine::ABallThrowerMachine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

}

// Called when the game starts or when spawned
void ABallThrowerMachine::BeginPlay()
{
	Super::BeginPlay();

	BindTimer();
	
}

void ABallThrowerMachine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (SpawnBallTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnBallTimerHandle);
	}
}

TObjectPtr<class ABall> ABallThrowerMachine::SpawnBall() const
{
	const FVector& ForwardVector = GetActorForwardVector();
	FVector Location = GetActorLocation() + (ForwardVector * 50.f) + FVector::UpVector * 50.f;

	if (StaticMeshComponent != nullptr)
	{
		Location = StaticMeshComponent->GetSocketLocation("Socket_Ball");
	}
	
	if (ABall* Ball = GetWorld()->SpawnActor<ABall>(BallClass, Location, GetActorRotation()))
	{
		return Ball;
	}

	return nullptr;
}

void ABallThrowerMachine::BindTimer()
{
	FTimerDelegate Delegate;
	Delegate.BindLambda([this]()
	{
		TObjectPtr<class AActor> Target = PickTarget();
		switch (BallThrowerShotType->GetInt())
		{
			case 0:
			if (FMath::RandBool())
			{
				GenerateFlatShot(Target->GetActorLocation());
			}
			else
			{
				GenerateTopSpinShot(Target->GetActorLocation());
			}
			break;
			
			case 1:
			GenerateFlatShot(Target->GetActorLocation());
			break;
			
			case 2:
			GenerateTopSpinShot(Target->GetActorLocation());
			break;
			
			case 3:
			GenerateSliceShot(Target->GetActorLocation());
			break;
			
			default:
			break;
		}
		
	});
	
	GetWorld()->GetTimerManager().SetTimer(SpawnBallTimerHandle, Delegate, SpawnFrequency, true, SpawnFrequency);
}

TObjectPtr<class AActor> ABallThrowerMachine::PickTarget()
{
	if (BallThrowerMode->GetInt() == 0)
	{
		if (!TargetActors.IsEmpty())
		{
			return TargetActors[FMath::RandRange(0, TargetActors.Num() - 1)];
		}
	}
	
	return TargetActor;
}

// Called every frame
void ABallThrowerMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void ABallThrowerMachine::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (UWorld* World = GetWorld())
	{
		if (World->IsPlayInEditor())
		{
			if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ABallThrowerMachine, SpawnFrequency))
			{
				if (SpawnBallTimerHandle.IsValid())
				{
					GetWorld()->GetTimerManager().ClearTimer(SpawnBallTimerHandle);
					BindTimer();
				}
			}	
		}
	}
}
#endif

void ABallThrowerMachine::GenerateFlatShot(const FVector& InTargetLocation)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, "Flat");
	
	if (const TObjectPtr<ABall> Ball = SpawnBall())
	{
		Ball->SetLifeSpan(10.f);

		if (const TObjectPtr<UBallProjectileMovementComponent> BallMovementComponent = Ball->GetComponentByClass<UBallProjectileMovementComponent>())
		{
			BallMovementComponent->DoFlatShot(InTargetLocation);
		}
	}
}

void ABallThrowerMachine::GenerateTopSpinShot(const FVector& InTargetLocation)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, "TopSpin");
	
	if (const TObjectPtr<ABall> Ball = SpawnBall())
	{
		Ball->SetLifeSpan(10.f);

		if (const TObjectPtr<UBallProjectileMovementComponent> BallMovementComponent = Ball->GetComponentByClass<UBallProjectileMovementComponent>())
		{
			BallMovementComponent->DoTopSpinShot(InTargetLocation);
		}
	}
}

void ABallThrowerMachine::GenerateSliceShot(const FVector& InTargetLocation)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Blue, "Slice");
	
	if (const TObjectPtr<ABall> Ball = SpawnBall())
	{
		Ball->SetLifeSpan(10.f);

		if (const TObjectPtr<UBallProjectileMovementComponent> BallMovementComponent = Ball->GetComponentByClass<UBallProjectileMovementComponent>())
		{
			BallMovementComponent->DoSliceShot(InTargetLocation);
		}
	}
}

