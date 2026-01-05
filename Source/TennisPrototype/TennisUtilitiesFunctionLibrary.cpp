// Fill out your copyright notice in the Description page of Project Settings.


#include "TennisUtilitiesFunctionLibrary.h"

FVector UTennisUtilitiesFunctionLibrary::FindRequiredVelocityAnalytic(const FVector& StartPos, const FVector& TargetPos,
	const FVector& AngularVelocity, float MagnusCoefficient, float GravityZ)
{
	// Time of flight (solve quadratic equation for Z-axis)
	float DeltaZ = TargetPos.Z - StartPos.Z;
	float A = -0.5f * GravityZ;
	float C = DeltaZ;

	// We need to solve for v₀ (initial velocity) in two steps:
	// 1. Vertical (Z-axis): Time depends on v_z.
	// 2. Horizontal (XY-plane): v_xy depends on time and Magnus effect.

	// Step 1: Choose v_z to ensure the ball reaches the target height.
	// For simplicity, assume v_z = sqrt(2*g*Δz) (min energy trajectory)
	float Vz = FMath::Sqrt(2.f * FMath::Abs(GravityZ * DeltaZ));

	// Step 2: Time of flight from vertical motion
	float Time = (Vz + FMath::Sqrt(Vz*Vz + 2*GravityZ*StartPos.Z)) / GravityZ;

	// Step 3: Solve for v_xy in the XY plane, accounting for Magnus effect
	FVector HorizontalDelta = (TargetPos - StartPos);
	// HorizontalDelta.Z = 0;

	// Required horizontal velocity to cover Δx, accounting for Magnus deflection
	// Δx = v_xy * t + 0.5 * a_magnus * t²
	// a_magnus = S * (ω × v) ≈ S * (ω × v_xy) (since v_z is small in XY plane)
	// Approximate solution: v_xy ≈ Δx / t - 0.5 * S * (ω × Δx)
	FVector Vxy = HorizontalDelta / Time - 
				 0.5f * MagnusCoefficient * FVector::CrossProduct(AngularVelocity, HorizontalDelta);

	return FVector(Vxy.X, Vxy.Y, Vz);
}

void UTennisUtilitiesFunctionLibrary::DrawPredictedPath(UObject* WorldContextObject, const FVector& StartPos, const FVector& Velocity, const FVector& Spin, const float MagnusCoefficient, float Duration = 2.0f)
{
	float TimeStep = 0.05f;
	FVector CurrentPos = StartPos;
	FVector CurrentVel = Velocity;

	for (float t = 0; t < Duration; t += TimeStep) {
		FVector NextPos = CurrentPos + CurrentVel * TimeStep;
		DrawDebugLine(WorldContextObject->GetWorld(), CurrentPos, NextPos, FColor::Green, false, 5.0f);
        
		// Apply gravity and Magnus effect
		CurrentVel.Z -= 980.f * TimeStep;
		FVector MagnusForce = MagnusCoefficient * FVector::CrossProduct(Spin, CurrentVel);
		CurrentVel += MagnusForce * TimeStep;
		CurrentPos = NextPos;
	}
}

FVector UTennisUtilitiesFunctionLibrary::IntegrateBall(
	FVector InitialVelocity,
	FVector StartPos,
	FVector AngularVelocity,
	float MagnusCoefficient,
	float GravityZ,
	float SimTime,
	float DT)
{
	FVector Pos = StartPos;
	FVector Vel = InitialVelocity;

	float Time = 0.f;

	while (Time < SimTime)
	{
		FVector Accel = FVector(0, 0, GravityZ)
					  + MagnusCoefficient * FVector::CrossProduct(AngularVelocity, Vel);

		Vel += Accel * DT;
		Pos += Vel * DT;

		Time += DT;
	}

	return Pos;
}

FVector UTennisUtilitiesFunctionLibrary::FindShotVelocity(
	const FVector& StartPos,
	const FVector& TargetPos,
	float ShotSpeed,
	const FVector& AngularVelocity,
	float MagnusCoefficient,
	float GravityZ)
{
	const float DesiredTime = (TargetPos - StartPos).Size() / ShotSpeed;

	// Initial guess: fire straight at target
	FVector AimDir = (TargetPos - StartPos).GetSafeNormal();

	for (int Iter = 0; Iter < 20; Iter++)
	{
		FVector TestVel = AimDir * ShotSpeed;

		FVector EndPos = IntegrateBall(
			TestVel, StartPos,
			AngularVelocity,
			MagnusCoefficient,
			GravityZ,
			DesiredTime,
			0.005f   // 5 ms timestep
		);

		FVector Error = TargetPos - EndPos;

		if (Error.Size() < 2.f) // tolerance
			break;

		// Adjust aim direction proportional to miss vector
		AimDir += Error.GetSafeNormal() * 0.1f;
		AimDir.Normalize();
	}

	return AimDir * ShotSpeed;
}
