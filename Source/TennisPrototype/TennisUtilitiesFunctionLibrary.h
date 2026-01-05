// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TennisUtilitiesFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TENNISPROTOTYPE_API UTennisUtilitiesFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Tennis")
	static FVector FindRequiredVelocityAnalytic(
	const FVector& StartPos,
	const FVector& TargetPos,
	const FVector& AngularVelocity,
	float MagnusCoefficient,
	float GravityZ = 980.0f
);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "Tennis")
	static void DrawPredictedPath(UObject* WorldContextObject, const FVector& StartPos, const FVector& Velocity, const FVector& Spin,
	                       float MagnusCoefficient,
	                       float Duration);
	static FVector IntegrateBall(FVector InitialVelocity, FVector StartPos, FVector AngularVelocity, float MagnusCoefficient,
	                      float GravityZ, float SimTime, float DT);
	static FVector FindShotVelocity(const FVector& StartPos, const FVector& TargetPos, float ShotSpeed,
	                         const FVector& AngularVelocity, float MagnusCoefficient, float GravityZ);
};
