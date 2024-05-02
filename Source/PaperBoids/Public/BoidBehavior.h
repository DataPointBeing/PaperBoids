// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BoidBehavior.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PAPERBOIDS_API UBoidBehavior : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBoidBehavior();

	static TArray<USkeletalMeshComponent*> boidMeshes;

protected:
	USkeletalMeshComponent* thisMesh;
	int framesSoFar;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FVector BoidRuleI();
	FVector BoidRuleII();
	FVector BoidRuleIII();

	FVector BoidGoalSettingBreeze();

};
