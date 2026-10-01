#include "Mind/Memory/SovereignMemoryVaultComponent.h"
#include "Misc/Guid.h" // For FGuid

USovereignMemoryVaultComponent::USovereignMemoryVaultComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USovereignMemoryVaultComponent::BeginPlay()
{
    Super::BeginPlay();
}

void USovereignMemoryVaultComponent::BindFilament(const FAffectFilamentTag& Filament)
{
    FSovereignMemoryFilament NewFilament;
    NewFilament.EventType = TEXT("HapticTouch"); // Default type for now
    NewFilament.Region = Filament.SourceRegion;
    NewFilament.EmotionalSignature = Filament.EmotionalLabel;
    NewFilament.Timestamp = FDateTime::UtcNow();
    NewFilament.LinkedThread = Filament.MemoryLinkID; // Use the provided link ID

    StoredFilaments.Add(NewFilament);
    UE_LOG(LogTemp, Log, TEXT("[SovereignMemoryVault] Bound filament: Region=%s, Label=%s, LinkID=%s"),
        *Filament.SourceRegion, *Filament.EmotionalLabel, *Filament.MemoryLinkID);
}

void USovereignMemoryVaultComponent::BloomFilamentBasedOnSceneTag(FString NarrativeScene)
{
    for (FSovereignMemoryFilament& Filament : StoredFilaments)
    {
        if (NarrativeScene.Contains(Filament.Region))
        {
            Filament.EmotionalSignature += TEXT("_Recalled");
            Filament.Timestamp = FDateTime::UtcNow();
            UE_LOG(LogTemp, Log, TEXT("[SovereignMemoryVault] Bloomed filament: %s (Region: %s)"), *Filament.LinkedThread, *Filament.Region);
        }
    }
}

FString USovereignMemoryVaultComponent::GenerateNewMemoryID()
{
    // A simple unique ID generator
    return FGuid::NewGuid().ToString();
}
