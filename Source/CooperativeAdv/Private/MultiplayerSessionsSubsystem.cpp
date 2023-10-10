// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem() {
	createServerAfterDestroy = false;
	serverNameToFind = "";
	PrintString("MSS Constructor"); 
	mainMapName = "";
	mySessionName = FName("Co-op Adventure Session Name");
}
void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection){ 
	IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get();
	PrintString("Initialize"); 
	if (!onlineSubsystem)
	{
		return;
	}
	FString subsystemName = onlineSubsystem->GetSubsystemName().ToString();
	PrintString(subsystemName);
	sessionInterface = onlineSubsystem->GetSessionInterface();
	if (sessionInterface.IsValid())
	{
		PrintString("Session interface is valid");
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnCreateSessionComplete);
		sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnDestroySessionComplete);
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnFindSessionComplete);
		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnJoinSessionComplete);
		
	}
}
void UMultiplayerSessionsSubsystem::Deinitialize() {
	PrintString("Deinitialize"); 
}

void UMultiplayerSessionsSubsystem::CreateServer(FString serverName)
{
	PrintString(serverName);
	if (serverName.IsEmpty())
	{
		PrintString("Server name cannot be empty!"); 
		ServerCreateDel.Broadcast(false);
		return;
	}
	
	FNamedOnlineSession* existingSession = sessionInterface->GetNamedSession(mySessionName);
	if (existingSession)
	{
		FString msg = FString::Printf(TEXT("Session with name %s already exists, destroying it."), *mySessionName.ToString());
		PrintString(msg);
		createServerAfterDestroy = true;
		destroyServername = serverName;
		sessionInterface->DestroySession(mySessionName);
		return;
	}
	FOnlineSessionSettings onlineSessionSettings;
	onlineSessionSettings.bAllowJoinInProgress = true;
	onlineSessionSettings.bIsDedicated = false;
	onlineSessionSettings.bShouldAdvertise = true;
	onlineSessionSettings.NumPublicConnections = 2;
	onlineSessionSettings.bUseLobbiesIfAvailable = true;
	onlineSessionSettings.bUsesPresence = true;
	onlineSessionSettings.bAllowJoinViaPresence = true;
	
	bool isLAN = true;
	IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get();
	if (onlineSubsystem->GetSubsystemName().ToString().Equals("STEAM"))
	{
		isLAN = false;
	}
	onlineSessionSettings.bIsLANMatch = isLAN;
	onlineSessionSettings.Set(FName("SERVER_NAME"), serverName,EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	sessionInterface->CreateSession(0, mySessionName, onlineSessionSettings);
}

void UMultiplayerSessionsSubsystem::FindServer(FString serverName)
{
	PrintString(serverName);
	if (serverName.IsEmpty())
	{
		PrintString(FString::Printf(TEXT("Server name cannot be empty!")));
		return;
	}
	sessionSearch = MakeShareable(new FOnlineSessionSearch);
	bool isLAN = true;
	if (IOnlineSubsystem::Get()->GetSubsystemName().ToString().Equals("STEAM"))
	{
		isLAN = false;
	}
	sessionSearch->bIsLanQuery = isLAN;
	sessionSearch->MaxSearchResults = 9999;
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	serverNameToFind = serverName;
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}
void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccesful)
{
	PrintString(FString::Printf(TEXT("OnCreateSessionComplete: %d for session %s"), bWasSuccesful, SessionName));
	ServerCreateDel.Broadcast(true);
	if (bWasSuccesful)
	{
		GetWorld()->ServerTravel(FString::Printf(TEXT("%s?listen"),*mainMapName));
	}
}
void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccesful)
{
	if (createServerAfterDestroy)
	{
		createServerAfterDestroy = false;
		CreateServer(destroyServername);
	}
	
}
void UMultiplayerSessionsSubsystem::OnFindSessionComplete(bool bWasSuccesful)
{
	
	if (!bWasSuccesful)
	{
		PrintString(FString::Printf(TEXT("Someting went wrong while looking for sessions")));
		ServerFoundDel.Broadcast(false);
		return;
	}
	if (serverNameToFind.IsEmpty())
	{
		ServerFoundDel.Broadcast(false);
		return;
	}
	TArray<FOnlineSessionSearchResult> results = sessionSearch->SearchResults;
	FOnlineSessionSearchResult* correctResult = 0;
	if (results.Num() <= 0)
	{
		PrintString(FString::Printf(TEXT("Zero Sessions found.")));
		ServerFoundDel.Broadcast(false);
		return;
	}
	for (FOnlineSessionSearchResult result :results )
	{
		if (!result.IsValid())
		{
			continue;
		}

		FString serverName = "No-name";
		result.Session.SessionSettings.Get(FName("SERVER_NAME"), serverName);
		FString msg = FString::Printf(TEXT("ServerName: %s"), *serverName);
		if (serverName.Equals(serverNameToFind))
		{
			correctResult = &result;
		}
		PrintString(msg);
		break;
	}
	if (!correctResult)
	{
		PrintString(FString::Printf(TEXT("Couldnt find server %s"), *serverNameToFind)); 
		ServerFoundDel.Broadcast(false);
		return;
	}
	PrintString(FString::Printf(TEXT("xD sessions found")));
	sessionInterface->JoinSession(0, mySessionName, *correctResult);
	PrintString(FString::Printf(TEXT("%d sessions found."), results.Num()));
}
void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	PrintString("We are here"); 
	if (result != EOnJoinSessionCompleteResult::Success)
	{
		ServerFoundDel.Broadcast(false);
		PrintString("Couldnt join server"); return;
	}
	FString Adress = "";
	bool Success = sessionInterface->GetResolvedConnectString(mySessionName, Adress);
	if (!Success)
	{
		ServerFoundDel.Broadcast(false);
		PrintString("Couldnt resolve conection");
		return;
	}
	APlayerController* playerController = GetGameInstance()->GetFirstLocalPlayerController();
	if (playerController)
	{
		ServerFoundDel.Broadcast(true);
		playerController->ClientTravel(Adress, ETravelType::TRAVEL_Absolute);
		return;
	}
	ServerFoundDel.Broadcast(false);
}
void UMultiplayerSessionsSubsystem::PrintString(const FString& message)
{
	if (!GEngine)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Blue, message);
}



