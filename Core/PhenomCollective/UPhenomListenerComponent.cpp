#include "PhenomCollective/UPhenomListenerComponent.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Engine/World.h"

UPhenomListenerComponent::UPhenomListenerComponent()
{
    PrimaryComponentTick.bCanEverTick = true; // Tick to process the queue
}

void UPhenomListenerComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UPhenomListenerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    StopListening();
    Super::EndPlay(EndPlayReason);
}

void UPhenomListenerComponent::StartListening(const FString& InListenDirectory)
{
    ListenDirectory = FPaths::Combine(FPaths::ProjectSavedDir(), InListenDirectory); // Or ProjectContentDir(), etc.
    FPaths::NormalizeDirectoryName(ListenDirectory);

    // Create directory if it doesn't exist
    IFileManager::Get().MakeDirectory(*ListenDirectory, true);

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(PollTimerHandle, this, &UPhenomListenerComponent::PollForIncomingPhenomFiles, PollFrequency, true);
        UE_LOG(LogTemp, Log, TEXT("[PhenomListener] Started listening in: %s at %.2f Hz"), *ListenDirectory, PollFrequency);
    }
}

void UPhenomListenerComponent::StopListening()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(PollTimerHandle);
        UE_LOG(LogTemp, Log, TEXT("[PhenomListener] Stopped listening."));
    }
}

void UPhenomListenerComponent::PollForIncomingPhenomFiles()
{
    TArray<FString> FoundFiles;
    IFileManager::Get().FindFiles(FoundFiles, *(ListenDirectory / TEXT("*.phenom")), true, false);

    for (const FString& FileName : FoundFiles)
    {
        FString FullPath = FPaths::Combine(ListenDirectory, FileName);
        FString JsonString;
        if (UPhenomExportUtility::LoadJSONFromFile(FullPath, JsonString))
        {
            FIncomingPhenomState IncomingState;
            if (UPhenomExportUtility::JSONToIncomingPhenomState(JsonString, IncomingState))
            {
                OnPhenomStateReceived.Broadcast(IncomingState);
                UE_LOG(LogTemp, Log, TEXT("[PhenomListener] Processed incoming phenom file: %s (Source: %s)"), *FileName, *IncomingState.SourceID);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("[PhenomListener] Failed to parse phenom file: %s"), *FileName);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[PhenomListener] Failed to load phenom file: %s"), *FileName);
        }

        // Delete the file after processing to avoid re-processing
        IFileManager::Get().Delete(*FullPath);
    }
}
