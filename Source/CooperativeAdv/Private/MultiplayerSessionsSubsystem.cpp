// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem() {
	PrintString("MSS Constructor"); 
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
	}
}
void UMultiplayerSessionsSubsystem::Deinitialize() {
	PrintString("Deinitialize"); 
}

void UMultiplayerSessionsSubsystem::PrintString(const FString& message)
{
	if (!GEngine)
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Blue, message);
}
