
#include "WinArea.h"
#include "Components/BoxComponent.h"
#include "../CooperativeAdvCharacter.h"
AWinArea::AWinArea()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	triggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Area"));
	SetRootComponent(triggerArea);
	playersInArea = 0;

}

void AWinArea::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		triggerArea->OnComponentBeginOverlap.AddDynamic(this, &AWinArea::OnTriggerAreaBeginOverlap);
		triggerArea->OnComponentEndOverlap.AddDynamic(this, &AWinArea::OnTriggerAreaEndOverlap);
	}
	
	triggerArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

}
void AWinArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWinArea::OnTriggerAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("D"));
	ACooperativeAdvCharacter* player = Cast<ACooperativeAdvCharacter>(OtherActor);
	if (!player)
	{
		return;
	}
	playersInArea++;
	UE_LOG(LogTemp, Warning, TEXT("%d"), playersInArea);
	if (playersInArea == 2)
	{
		MulticastRPCWin();
	}
}

void AWinArea::OnTriggerAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACooperativeAdvCharacter* player = Cast<ACooperativeAdvCharacter>(OtherActor);
	if (!player)
	{
		return;
	}
	playersInArea--;
}

void AWinArea::MulticastRPCWin_Implementation()
{
	OnWinCondition.Broadcast();
}

