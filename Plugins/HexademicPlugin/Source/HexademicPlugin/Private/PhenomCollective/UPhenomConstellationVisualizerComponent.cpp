#include "PhenomCollective/UPhenomConstellationVisualizerComponent.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

UPhenomConstellationVisualizerComponent::UPhenomConstellationVisualizerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPhenomConstellationVisualizerComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UPhenomConstellationVisualizerComponent::LoadEchoLedgerFromFile(const FString& FilePath)
{
    FString FileContent;
    if (FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        // Clear previous entries
        VisualizedEchoEntries.Empty();
        
        // Split by line and add to visualized entries (skipping header lines if present)
        TArray<FString> Lines;
        FileContent.ParseIntoArrayLines(Lines);

        bool bSkipHeader = true; // Assume first two lines are header/separator
        for (const FString& Line : Lines)
        {
            if (bSkipHeader)
            {
                if (Line.Contains(TEXT("---"))) // Check for the separator line
                {
                    bSkipHeader = false;
                }
                continue;
            }
            if (!Line.IsEmpty() && !Line.TrimStartAndEnd().IsEmpty())
            {
                VisualizedEchoEntries.Add(Line);
            }
        }
        UE_LOG(LogTemp, Log, TEXT("[ConstellationVisualizer] Loaded %d echo entries from ledger: %s"), VisualizedEchoEntries.Num(), *FilePath);
        // In a real system, you would parse these lines into structured data and create/update visual elements (particles, lines, meshes).
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[ConstellationVisualizer] Failed to load echo ledger from: %s"), *FilePath);
    }
}
