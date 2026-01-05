// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TENNISPROTOTYPE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABall> BallClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> TargetActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USphereComponent> RadiusSphereComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void DoFlatShot();

	UFUNCTION(BlueprintCallable)
	void DoTopSpinShot();
	
	UFUNCTION(BlueprintCallable)
	void MoveTargetActor(const float InX, const float InY, const float InSpeedMultiplier);
	
	UFUNCTION(BlueprintCallable)
	void SetTargetActorLocation(const FVector& InLocation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShotBufferTime;
	
};
