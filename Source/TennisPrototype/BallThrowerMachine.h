// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallThrowerMachine.generated.h"

UCLASS()
class TENNISPROTOTYPE_API ABallThrowerMachine : public AActor
{
private:
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ABallThrowerMachine();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABall> BallClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> TargetActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<AActor>> TargetActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnFrequency;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	TObjectPtr<class ABall> SpawnBall() const;

private:

	FTimerHandle SpawnBallTimerHandle;

	UFUNCTION()
	void BindTimer();
	
	TObjectPtr<class AActor> PickTarget();
	
public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#if  WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UFUNCTION(BlueprintCallable)
	void GenerateFlatShot(const FVector& InTargetLocation);

	UFUNCTION(BlueprintCallable)
	void GenerateTopSpinShot(const FVector& InTargetLocation);
	
	UFUNCTION(BlueprintCallable)
	void GenerateSliceShot(const FVector& InTargetLocation);
	
	
};
