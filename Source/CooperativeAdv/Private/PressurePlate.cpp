// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"
#include "Components/StaticMeshComponent.h"
#include "Transporter.h"
// Sets default values
APressurePlate::APressurePlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	//transporter = CreateDefaultSubobject<UTransporter>(TEXT("Transporter"));
	////transporter->AddTriggerActor(this);
	//transporter->moveTime = 0.25;
	//transporter->SetPoints(GetActorLocation(), GetActorLocation() + FVector(0.0f, 0.0f, -10.0f));
	
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/Asian_Village/meshes/building/SM_column_decoration_03.SM_column_decoration_03"));
	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
		Mesh->SetRelativeScale3D(FVector(3.3f, 3.3f, 0.2f));
		Mesh->SetRelativeLocation(FVector::UpVector * 7.2f);
	}
	TriggerShape = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerShape"));
	TriggerShape->SetIsReplicated(true);
	
	TriggerShape->SetupAttachment(GetRootComponent());
	auto triggerMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));
	if (triggerMeshAsset.Succeeded())
	{
		TriggerShape->SetStaticMesh(triggerMeshAsset.Object);
		TriggerShape->SetRelativeScale3D(FVector(3.3f, 3.3f, 0.2f));
		TriggerShape->SetRelativeLocation(FVector::UpVector * 10.f);
		activated = false;
	}
}

void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	TriggerShape->SetVisibility(false);
	TriggerShape->SetCollisionProfileName(FName("OverlapAll"));
}

void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HasAuthority())
	{
		return;
	}
	AActor* trigerActor = NULL;
	TArray<AActor*> overlapingActors;
	TriggerShape->GetOverlappingActors(overlapingActors);
	for (int i = 0; i < overlapingActors.Num(); i++)
	{
		AActor* A = overlapingActors[i];
		FString msg = FString::Printf(TEXT("Name: %s"), *A->GetName());
		if (A->ActorHasTag("TriggerActor"))
		{
			trigerActor = A;
			break;
		}
	}
	if (trigerActor)
	{
		if (!activated)
		{
			activated = true;
			OnActivated.Broadcast();
		}
	}
	else {
		if (activated)
		{
			activated = false;
			OnDeactivated.Broadcast();
		}
	}
}

