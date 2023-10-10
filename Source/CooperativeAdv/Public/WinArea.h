// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WinArea.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWinAreaOnWinCondition);
UCLASS()
class COOPERATIVEADV_API AWinArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AWinArea();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	int playersInArea;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UBoxComponent* triggerArea;
	UPROPERTY(BlueprintReadWrite)
		bool winCondition;
	UFUNCTION()
	void OnTriggerAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnTriggerAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
		void MulticastRPCWin();
	UPROPERTY(BlueprintAssignable)
		FWinAreaOnWinCondition OnWinCondition;

};
