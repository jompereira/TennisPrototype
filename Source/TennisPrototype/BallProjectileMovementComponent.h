// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BallProjectileMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class TENNISPROTOTYPE_API UBallProjectileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector AngularVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagnusCoefficient = 0.0005f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DragCoefficient = 0.06f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DebugPath = false;

	UPROPERTY(editAnywhere, BlueprintReadWrite)
	bool DebugMagnusEffect = false;

private:

	FColor DebugPathColor = FColor::MakeRandomColor();
	
public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual FVector ComputeAcceleration(const FVector& InVelocity, float DeltaTime) const override;
	FVector ComputeDrag(const FVector& InVelocity, float DeltaTime) const;
	FVector ComputeMagnusEffectAcceleration(float DeltaTime) const;
	virtual FVector ComputeVelocity(FVector InitialVelocity, float DeltaTime) const override;

public:
	void ApplyForces(const FVector& InLinearImpulse, const FVector& InAngularImpulseInRadians);

	UFUNCTION(BlueprintCallable)
	void DoFlatShot(const FVector& InTargetLocation);

	UFUNCTION(BlueprintCallable)
	void DoTopSpinShot(const FVector& InTargetLocation);
	
	UFUNCTION(BlueprintCallable)
	void DoSliceShot(const FVector& InTargetLocation);
	
};
