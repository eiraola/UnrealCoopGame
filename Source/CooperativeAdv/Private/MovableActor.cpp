// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/SceneComponent.h"
#include "Transporter.h"
#include "Components/ArrowComponent.h"
#include "MovableActor.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
AMovableActor::AMovableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	bReplicates = true;
	SetReplicateMovement(true);
	PrimaryActorTick.bCanEverTick = true;
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(rootComp);
	pointA = CreateDefaultSubobject<UArrowComponent>(TEXT("Point one"));
	pointA->SetupAttachment(GetRootComponent());
	pointA->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	pointB = CreateDefaultSubobject<UArrowComponent>(TEXT("Point two"));
	pointB->SetupAttachment(GetRootComponent());
	pointB->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(GetRootComponent());
	mesh->SetIsReplicated(true);
	transporter = CreateDefaultSubobject<UTransporter>(TEXT("Transporter"));
}

// Called when the game starts or when spawned
void AMovableActor::BeginPlay()
{
	Super::BeginPlay();
	transporter->SetPoints(pointA->GetRelativeLocation() + GetActorLocation(), pointB->GetRelativeLocation() + GetActorLocation());
}

// Called every frame
void AMovableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

