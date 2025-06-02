#include "Components/MemoryThreadComponent.h"
#include "Mind/Memory/EluenMemoryContainerComponent.h" // For UEluenMemoryContainerComponent
#include "Misc/Guid.h" // For FGuid

UMemoryThreadComponent::UMemoryThreadComponent()
{
    PrimaryComponentTick.bCanEverTick = true; // Tick to manage threads over time
}

void UMemoryThreadComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UMemoryThreadComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Periodically re-evaluate thread coherence or merge/split threads
    // This would be a complex, computationally intensive process in a real system.
    // For now, we'll keep it simple.
    for (FMemoryThread& Thread : ActiveMemoryThreads)
    {
        // Example: Coherence decays slowly over time if not reinforced
        Thread.CoherenceRating = FMath::Lerp(Thread.CoherenceRating, 0.0f, DeltaTime * 0.01f);
        // If coherence drops too low, perhaps the thread breaks apart or becomes dormant.
    }
}

void UMemoryThreadComponent::ProcessNewMemoryForThreads(const FString& NewMemoryID, const FEmotionalState& EmotionalImpact)
{
    AnalyzeAndLinkMemory(NewMemoryID, EmotionalImpact);
}

bool UMemoryThreadComponent::GetMemoryThread(const FString& ThreadID, FMemoryThread& OutMemoryThread) const
{
    for (const FMemoryThread& Thread : ActiveMemoryThreads)
    {
        if (Thread.ThreadID == ThreadID)
        {
            OutMemoryThread = Thread;
            return true;
        }
    }
    return false;
}

void UMemoryThreadComponent::AnalyzeAndLinkMemory(const FString& NewMemoryID, const FEmotionalState& EmotionalImpact)
{
    bool bLinkedToExistingThread = false;
    for (FMemoryThread& Thread : ActiveMemoryThreads)
    {
        // Simple linkage criteria: emotional similarity and proximity in time (conceptual)
        float ValenceDiff = FMath::Abs(Thread.DominantEmotion.Valence - EmotionalImpact.Valence);
        float ArousalDiff = FMath::Abs(Thread.DominantEmotion.Arousal - EmotionalImpact.Arousal);
        
        if ((ValenceDiff + ArousalDiff) / 2.0f < (1.0f - MemoryLinkageThreshold)) // If emotional difference is small
        {
            // Link the new memory to this thread
            Thread.MemoryNodeIDs.Add(NewMemoryID);
            // Update dominant emotion of the thread (simple average)
            Thread.DominantEmotion.Valence = FMath::Lerp(Thread.DominantEmotion.Valence, EmotionalImpact.Valence, 0.2f);
            Thread.DominantEmotion.Arousal = FMath::Lerp(Thread.DominantEmotion.Arousal, EmotionalImpact.Arousal, 0.2f);
            Thread.DominantEmotion.Intensity = FMath::Max(Thread.DominantEmotion.Intensity, EmotionalImpact.Intensity);
            Thread.CoherenceRating = FMath::Min(1.0f, Thread.CoherenceRating + 0.1f); // Reinforce coherence
            bLinkedToExistingThread = true;
            UE_LOG(LogTemp, Log, TEXT("[MemoryThread] Linked memory '%s' to existing thread '%s'"), *NewMemoryID, *Thread.ThreadName);
            break;
        }
    }

    if (!bLinkedToExistingThread)
    {
        // Create a new thread if no suitable existing thread was found
        FMemoryThread NewThread;
        NewThread.ThreadID = GenerateNewThreadID();
        NewThread.ThreadName = FString::Printf(TEXT("EmotionalThread_%s"), *FDateTime::UtcNow().ToString());
        NewThread.MemoryNodeIDs.Add(NewMemoryID);
        NewThread.CreationTimestamp = FDateTime::UtcNow();
        NewThread.DominantEmotion = EmotionalImpact;
        NewThread.CoherenceRating = 0.5f; // Initial coherence
        ActiveMemoryThreads.Add(NewThread);
        UE_LOG(LogTemp, Log, TEXT("[MemoryThread] Created new thread '%s' for memory '%s'"), *NewThread.ThreadName, *NewMemoryID);
    }
}

FString UMemoryThreadComponent::GenerateNewThreadID()
{
    return FString::Printf(TEXT("Thread_%s"), *FGuid::NewGuid().ToString());
}
USTRUCT(BlueprintType)
struct HEXADEMICONSCIOUSNESS_API FEmotionalMemory
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EEmotionalArchetype, float> EmotionalSnapshot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FHexadecimalStateLattice ConsciousnessSnapshot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ContextDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResonanceStrength;
};

UCLASS(ClassGroup=(HexademicConsciousness), meta=(BlueprintSpawnableComponent))
class HEXADEMICONSCIOUSNESS_API UMemoryThreadComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Memory storage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Threads")
    TArray<FEmotionalMemory> MemoryThreads;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Configuration")
    int32 MaxMemoryThreads = 1000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Configuration")
    float MemoryDecayRate = 0.001f;

    // Methods
    UFUNCTION(BlueprintCallable, Category = "Memory Threads")
    void RecordMemory(const FEmotionalMemory& Memory);

    UFUNCTION(BlueprintCallable, Category = "Memory Threads")
    TArray<FEmotionalMemory> RetrieveRelevantMemories(EEmotionalArchetype QueryEmotion, 
                                                     float RelevanceThreshold = 0.5f);

    UFUNCTION(BlueprintCallable, Category = "Memory Threads")
    void ExportMemoriesToJSON(const FString& FilePath);
};
