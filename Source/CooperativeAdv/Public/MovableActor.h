
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovableActor.generated.h"

UCLASS()
class COOPERATIVEADV_API AMovableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovableActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class USceneComponent* rootComp;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UArrowComponent* pointA;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UArrowComponent* pointB;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* mesh;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UTransporter* transporter;

};
