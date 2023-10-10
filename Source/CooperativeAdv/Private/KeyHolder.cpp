// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyHolder.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
AKeyHolder::AKeyHolder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(rootComp);
	keyMeshOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("KeyPoint"));
	keyMeshOrigin->SetupAttachment(GetRootComponent());
	keyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key Mesh"));
	keyMesh->SetupAttachment(keyMeshOrigin);
	keyMesh->SetIsReplicated(true);
	keyMesh->SetCollisionProfileName(FName("OverlapAllDynamic"));
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(GetRootComponent());
	mesh->SetIsReplicated(true);
	mesh->SetCollisionProfileName(FName("BlockAllDynamic"));
	rotationSpeed = 100.0f;
}

// Called when the game starts or when spawned
void AKeyHolder::BeginPlay()
{
	Super::BeginPlay();
	keyMesh->SetVisibility(false);
}

// Called every frame
void AKeyHolder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	keyMeshOrigin->AddRelativeRotation(FRotator(0.0, rotationSpeed * DeltaTime, 0.0f));
}

void AKeyHolder::ActivateKeyMesh()
{
	keyMesh->SetVisibility(true);
}

