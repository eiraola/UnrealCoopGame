// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Transporter.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPERATIVEADV_API UTransporter : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTransporter();

protected:
	virtual void BeginPlay() override;
	virtual void Transport(float deltaTime);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FVector startPoint;
	FVector endPoint;
	AActor* owner;
	bool bArePointsSet;
	UPROPERTY(EditAnywhere)
	float moveTime;
	UPROPERTY(EditAnywhere)
	TArray<AActor*> triggerActors;
	UPROPERTY(VisibleAnywhere)
	int ActivatedTriggerCount;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool allTriggerActorsTriggered;
	UFUNCTION(BlueprintCallable)
		void SetPoints(FVector point1, FVector point2);
	UFUNCTION(BlueprintCallable)
	void OnPressurePlateActivated();
	UFUNCTION(BlueprintCallable)
	void OnPressurePlateDeactivated();

		
};
