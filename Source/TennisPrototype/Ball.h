// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UENUM(BlueprintType)
enum class EShotType : uint8
{
	FLAT,
	TOPSPIN,
	SLICE,
	LOB
};

UCLASS()
class TENNISPROTOTYPE_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBallProjectileMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagnusCoefficient = 0.005f;
	
	UFUNCTION(BlueprintCallable)
	void Hit(const FVector& InTargetLocation, const EShotType InShotType);
	
};
