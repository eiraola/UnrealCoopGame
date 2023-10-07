// Fill out your copyright notice in the Description page of Project Settings.


#include "Transporter.h"
#include "PressurePlate.h"
// Sets default values for this component's properties
UTransporter::UTransporter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	moveTime = 3.0f;
	ActivatedTriggerCount = 0;
	bArePointsSet = false;
	allTriggerActorsTriggered = false;
	startPoint = FVector(0.0, 0.0, 0.0);
	endPoint = FVector(0.0, 0.0, 0.0);
	// ...
}


// Called when the game starts
void UTransporter::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner();
	for (AActor* TA : triggerActors)
	{
		APressurePlate* pressurePlate = Cast<APressurePlate>(TA);
		if (pressurePlate)
		{
			pressurePlate->OnActivated.AddDynamic(this, &UTransporter::OnPressurePlateActivated);
			pressurePlate->OnDeactivated.AddDynamic(this, &UTransporter::OnPressurePlateDeactivated);
		}
	}
	
}

void UTransporter::Transport(float deltaTime)
{
	if (!owner || !owner->HasAuthority() || !bArePointsSet)
	{
		return;
	}
	FVector currentLocation = owner->GetActorLocation();
	float speed = FVector::Distance(startPoint, endPoint) / moveTime;
	FVector targetLocation = allTriggerActorsTriggered ? endPoint : startPoint;
	if (currentLocation.Equals(targetLocation))
	{
		return;
	}
	FVector newLocation = FMath::VInterpConstantTo(currentLocation, targetLocation, deltaTime, speed);
	owner->SetActorLocation(newLocation);
}


// Called every frame
void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Transport(DeltaTime);
}

void UTransporter::SetPoints(FVector point1, FVector point2)
{
	startPoint = point1;
	endPoint = point2;
	bArePointsSet = true;
}

void UTransporter::OnPressurePlateActivated()
{
	ActivatedTriggerCount++;
	allTriggerActorsTriggered = (ActivatedTriggerCount >= triggerActors.Num());

	FString msg = FString::Printf(TEXT("Transporter Activated. %d"), ActivatedTriggerCount);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, msg);
}

void UTransporter::OnPressurePlateDeactivated()
{
	ActivatedTriggerCount--;
	allTriggerActorsTriggered = (ActivatedTriggerCount >= triggerActors.Num());
	FString msg = FString::Printf(TEXT("Transporter Activated. %d"), ActivatedTriggerCount);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, msg);
}

