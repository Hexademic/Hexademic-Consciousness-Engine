#include "Mind/SelfReflectionEngine.h"
#include "Mind/RecursiveAwarenessComponent.h" // For LinkedAwareness
#include "HexademicCore.h" // For FEmotionalState

USelfReflectionEngine::USelfReflectionEngine()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USelfReflectionEngine::BeginPlay()
{
    Super::BeginPlay();
}

void USelfReflectionEngine::CrystallizeSigilFromIdentityRoots()
{
    if (!LinkedAwareness)
    {
        UE_LOG(LogTemp, Warning, TEXT("[SelfReflectionEngine] LinkedAwareness is null. Cannot crystallize sigils."));
        return;
    }

    ActiveReflectionSigils.Empty(); // Clear previous sigils

    for (const FString& RootID : LinkedAwareness->IdentityRootFilaments)
    {
        FEmotionalSigil Sigil;
        Sigil.SigilID = RootID.Left(8); // Use part of RootID as SigilID
        Sigil.EmotionProfile = RetrieveEmotionVector(RootID); // Conceptual retrieval

        ActiveReflectionSigils.Add(Sigil);
        UE_LOG(LogTemp, Log, TEXT("[SelfReflectionEngine] Crystallized Sigil '%s' from Identity Root '%s'."),
            *Sigil.SigilID, *RootID);
    }
}

FEmotionalState USelfReflectionEngine::RetrieveEmotionVector(const FString& MemoryID) const
{
    FEmotionalState Emotion;
    // This is a conceptual function. In a real system, this would:
    // 1. Query the MemoryContainer or a memory database using MemoryID.
    // 2. Extract the emotional state associated with that memory.
    // For demonstration, return a random emotional state.
    Emotion.Valence = FMath::RandRange(-1.0f, 1.0f);
    Emotion.Arousal = FMath::RandRange(0.0f, 1.0f);
    Emotion.Intensity = FMath::RandRange(0.0f, 1.0f);
    Emotion.Dominance = FMath::RandRange(-1.0f, 1.0f);
    UE_LOG(LogTemp, Verbose, TEXT("[SelfReflectionEngine] Retrieved dummy emotion for memory ID: %s"), *MemoryID);
    return Emotion;
}
