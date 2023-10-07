// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MultiplayerSessionsSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerCreateDelegate, bool, WasSuccesful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerFoundDelegate, bool, WasSuccesful);
/**
 * 
 */
UCLASS()
class COOPERATIVEADV_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UMultiplayerSessionsSubsystem();
	UPROPERTY(BlueprintReadWrite)
		FString mainMapName;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void PrintString(const FString& message);
	IOnlineSessionPtr sessionInterface;
	UFUNCTION(BlueprintCallable)
		void CreateServer(FString serverName);
	UFUNCTION(BlueprintCallable)
		void FindServer(FString serverName);
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccesful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccesful);
	void OnFindSessionComplete(bool bWasSuccesful);
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);
private:
	bool createServerAfterDestroy;
	FString destroyServername;
	FString serverNameToFind;
	FName mySessionName;
	TSharedPtr<FOnlineSessionSearch> sessionSearch;
	UPROPERTY(BlueprintAssignable)
		FServerCreateDelegate ServerCreateDel;
	UPROPERTY(BlueprintAssignable)
		FServerFoundDelegate ServerFoundDel;
		
};
