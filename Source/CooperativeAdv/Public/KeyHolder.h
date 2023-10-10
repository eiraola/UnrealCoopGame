// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyHolder.generated.h"

UCLASS()
class COOPERATIVEADV_API AKeyHolder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeyHolder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float rotationSpeed;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class USceneComponent* rootComp;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class USceneComponent* keyMeshOrigin;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class UStaticMeshComponent* mesh;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class UStaticMeshComponent* keyMesh;
	UFUNCTION()
		void ActivateKeyMesh();
};
