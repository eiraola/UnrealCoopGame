// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableKey.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "../CooperativeAdvCharacter.h"
#include "Components/AudioComponent.h"
#include "KeyHolder.h"
// Sets default values
ACollectableKey::ACollectableKey()
{
	bReplicates = true;
	SetReplicateMovement(true);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(rootComp);
	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	capsule->SetupAttachment(GetRootComponent());
	capsule->SetIsReplicated(true);
	capsule->SetCapsuleHalfHeight(150.0f);
	capsule->SetCapsuleRadius(100.0f);
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(GetRootComponent());
	mesh->SetIsReplicated(true);
	mesh->SetCollisionProfileName(FName("OverlapAllDynamic"));
	IsCollected = false;
	rotationSpeed = 100.0f;
	pickSound = CreateDefaultSubobject<UAudioComponent>(TEXT("CollectAudio"));
	pickSound->SetupAttachment(GetRootComponent());
	pickSound->SetAutoActivate(false);
}



// Called when the game starts or when spawned
void ACollectableKey::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		capsule->OnComponentBeginOverlap.AddDynamic(this, &ACollectableKey::OnCapsuleBeginOverlap);
	}
	
}

// Called every frame
void ACollectableKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	mesh->AddRelativeRotation(FRotator(0.0, rotationSpeed * DeltaTime, 0.0f));
}
void ACollectableKey::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACollectableKey, IsCollected);
}
void ACollectableKey::OnRep_IsCollected()
{
	UE_LOG(LogTemp, Warning, TEXT("CollectableKey: Called"));
	mesh->SetVisibility(false);
	pickSound->Play();
	if (keyHolderRef)
	{
		keyHolderRef->ActivateKeyMesh();
	}
	if (HasAuthority() && IsCollected)
	{
		onKeyCollected.Broadcast();
	}
}

void ACollectableKey::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("CollectableKey: Collided"));
	if (IsCollected)
	{
		return;
	}
	ACooperativeAdvCharacter* player = Cast <ACooperativeAdvCharacter>(OtherActor);
	if (!player)
	{
		UE_LOG(LogTemp, Warning, TEXT("CollectableKey: Isnt a player"));
		return;
	}
	IsCollected = true;
	OnRep_IsCollected();
}

