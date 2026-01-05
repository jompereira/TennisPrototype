// Fill out your copyright notice in the Description page of Project Settings.


#include "BallProjectileMovementComponent.h"

#include "TennisUtilitiesFunctionLibrary.h"
#include "Field/FieldSystem.h"
#include "Misc/MapErrors.h"

static FAutoConsoleVariable FlatShotAngularImpulseCVar(
						TEXT("Ball.AngularImpulse.Flat"),
						300.f,
						TEXT("The angular impulse of a flat shot."));

static FAutoConsoleVariable FlatShotLinearImpulseCVar(
						TEXT("Ball.LinearImpulse.Flat"),
						300.f,
						TEXT("The linear impulse of a flat shot."));

static FAutoConsoleVariable TopSpinShotAngularImpulseCVar(
						TEXT("Ball.AngularImpulse.TopSpin"),
						-50.f,
						TEXT("The angular impulse of a top spin shot."));

static FAutoConsoleVariable TopSpinShotLinearImpulseCVar(
						TEXT("Ball.LinearImpulse.TopSpin"),
						250.f,
						TEXT("The linear impulse of a top spin shot."));
						
static FAutoConsoleVariable SliceShotAngularImpulseCVar(
					TEXT("Ball.AngularImpulse.Slice"),
					150.f,
					TEXT("The angular impulse of a top spin shot."));

static FAutoConsoleVariable SliceShotLinearImpulseMultiplierCVar(
					TEXT("Ball.LinearImpulseMultiplier.Slice"),
					1.f,
					TEXT("The linear impulse of a top spin shot."));


void UBallProjectileMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (DebugPath)
	{
		DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), 4.f, 16, DebugPathColor, false, 0.5f);
	}
}

FVector UBallProjectileMovementComponent::ComputeAcceleration(const FVector& InVelocity, float DeltaTime) const
{
	return Super::ComputeAcceleration(InVelocity, DeltaTime) +
			ComputeMagnusEffectAcceleration(DeltaTime)		 +
			ComputeDrag(InVelocity, DeltaTime);
}

FVector UBallProjectileMovementComponent::ComputeDrag(const FVector& InVelocity, float DeltaTime) const
{
	return -DragCoefficient * InVelocity * InVelocity.Size();
}

FVector UBallProjectileMovementComponent::ComputeMagnusEffectAcceleration(float DeltaTime) const
{
	if (!Velocity.IsNearlyZero())
	{
		FVector MagnusEffect = FVector::CrossProduct(Velocity, AngularVelocity) * MagnusCoefficient * DeltaTime;

		if (DebugMagnusEffect)
		{
			DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + MagnusEffect * 50.f, FColor::Red);
		}

		return MagnusEffect;
	}
	
	return FVector::ZeroVector;
}

FVector UBallProjectileMovementComponent::ComputeVelocity(FVector InitialVelocity, float DeltaTime) const
{
	return Super::ComputeVelocity(InitialVelocity, DeltaTime); 
}

void UBallProjectileMovementComponent::ApplyForces(const FVector& InLinearImpulse,
	const FVector& InAngularImpulseInRadians)
{
	AngularVelocity = InAngularImpulseInRadians;
	SetVelocityInLocalSpace(GetOwner()->GetTransform().InverseTransformVector(InLinearImpulse));
}

void UBallProjectileMovementComponent::DoFlatShot(const FVector& InTargetLocation)
{
	constexpr static float FlatShotAngularImpulse = 50.f;

	if (TObjectPtr<AActor> Owner = GetOwner())
	{
		const FVector NewAngularVelocityInRadians = Owner->GetActorRightVector() * /**FlatShotAngularImpulse*/ FlatShotAngularImpulseCVar->GetFloat();
		const FVector& ActorLocation = Owner->GetActorLocation();

		FVector NewVelocity = UTennisUtilitiesFunctionLibrary::FindRequiredVelocityAnalytic(
				ActorLocation, InTargetLocation, NewAngularVelocityInRadians, MagnusCoefficient);

		ApplyForces(NewVelocity, NewAngularVelocityInRadians);
	}
}

void UBallProjectileMovementComponent::DoTopSpinShot(const FVector& InTargetLocation)
{
	constexpr static float TopSpinShotAngularImpulse = -1000.f;

	if (TObjectPtr<AActor> Owner = GetOwner())
	{
		const FVector NewAngularVelocityInRadians = Owner->GetActorRightVector() * /**-TopSpinShotAngularImpulse*/ TopSpinShotAngularImpulseCVar->GetFloat();
			
		const FVector& NewVelocity = UTennisUtilitiesFunctionLibrary::FindRequiredVelocityAnalytic(
			Owner->GetActorLocation(), InTargetLocation, NewAngularVelocityInRadians, MagnusCoefficient);

		ApplyForces(NewVelocity, NewAngularVelocityInRadians);
	}
}

void UBallProjectileMovementComponent::DoSliceShot(const FVector& InTargetLocation)
{
	if (TObjectPtr<AActor> Owner = GetOwner())
	{
		const FVector NewAngularVelocityInRadians = Owner->GetActorRightVector() * SliceShotAngularImpulseCVar->GetFloat();
		const FVector& ActorLocation = Owner->GetActorLocation();

		FVector NewVelocity = UTennisUtilitiesFunctionLibrary::FindRequiredVelocityAnalytic(
				ActorLocation, InTargetLocation, NewAngularVelocityInRadians, MagnusCoefficient);

		NewVelocity = FVector((NewVelocity.GetSafeNormal() * (NewVelocity.Size() * SliceShotLinearImpulseMultiplierCVar->GetFloat())).X,
								(NewVelocity.GetSafeNormal() * (NewVelocity.Size() * SliceShotLinearImpulseMultiplierCVar->GetFloat())).Y,
								NewVelocity.Z);
		ApplyForces(NewVelocity, NewAngularVelocityInRadians);
	}
}






















