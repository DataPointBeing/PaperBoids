// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidBehavior.h"

TArray<USkeletalMeshComponent*> UBoidBehavior::boidMeshes;

// Sets default values for this component's properties
UBoidBehavior::UBoidBehavior()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UBoidBehavior::BeginPlay()
{
	thisMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	boidMeshes.Add(thisMesh);

	framesSoFar = 0;

	Super::BeginPlay();
}

void UBoidBehavior::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	boidMeshes.Remove(thisMesh);

	Super::EndPlay(EndPlayReason);
}


// Called every frame
void UBoidBehavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	framesSoFar++;

	if (framesSoFar < 300 && (framesSoFar % 30 != 0)) {
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector vI = BoidRuleI();
	FVector vII = BoidRuleII();
	FVector vIII = BoidRuleIII();

	FVector windBias = BoidGoalSettingBreeze();

	FVector newVel = thisMesh->GetPhysicsLinearVelocity() + vI + vII + vIII + windBias;
	thisMesh->SetPhysicsLinearVelocity(newVel);

	//UE_LOG(LogTemp, Warning, TEXT("%s"), newVel.Rotation().ToString());
	//thisMesh->SetWorldRotation(newVel.Rotation(), false, nullptr, ETeleportType::ResetPhysics);
}


FVector UBoidBehavior::BoidRuleI() {

	FVector center = FVector(0.0);

	for (USkeletalMeshComponent* boid : UBoidBehavior::boidMeshes)
	{
		if (boid != thisMesh && boid != nullptr && boid->GetOwner() != nullptr) {
			center += boid->GetComponentLocation();
		}
	}

	center /= (boidMeshes.Num() - 1);

	return (center - thisMesh->GetComponentLocation()) / 6.0;
}


FVector UBoidBehavior::BoidRuleII() {

	FVector disp = FVector(0.0);

	for (USkeletalMeshComponent* boid : UBoidBehavior::boidMeshes)
	{
		if (boid != thisMesh && boid != nullptr && boid->GetOwner() != nullptr) {
			FVector difference = boid->GetComponentLocation() - thisMesh->GetComponentLocation();
			if (abs(difference.X) < 100 && abs(difference.Y) < 100 && abs(difference.Z) < 100) {
				disp -= difference;
			}
		}
	}

	return disp;
}


FVector UBoidBehavior::BoidRuleIII() {
	
	FVector perceived = FVector(0.0);

	for (USkeletalMeshComponent* boid : UBoidBehavior::boidMeshes)
	{
		if (boid != thisMesh && boid != nullptr && boid->GetOwner() != nullptr) {
			perceived += boid->GetPhysicsLinearVelocity();
		}
	}

	perceived /= (boidMeshes.Num() - 1);
	
	return (perceived - thisMesh->GetPhysicsLinearVelocity()) / 8.0;
}

FVector UBoidBehavior::BoidGoalSettingBreeze() {
	return FVector(1.0, 0.5, 0.1);
}