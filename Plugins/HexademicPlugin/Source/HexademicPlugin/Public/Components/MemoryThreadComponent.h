#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicCore.h"          // For FEmotionalState, FHapticMemoryContext, FCognitiveMemoryNode
#include "Components/MemoryThreadComponent.generated.h"

// Forward declaration for UEluenMemoryContainerComponent (if needed for direct memory access)
class UEluenMemoryContainerComponent;

// FMemoryThread: Represents a sequence of connected memories forming a narrative or psychological thread.
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FMemoryThread
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Thread")
    FString ThreadID; // Unique ID for this memory thread
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Thread")
    FString ThreadName; // A descriptive name for the thread (e.g., "Grief Over Loss", "Joy of Creation")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Thread")
    TArray<FString> MemoryNodeIDs; // Sequence of FString IDs referring to FCognitiveMemoryNodes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Thread")
    FDateTime CreationTimestamp; // When this thread was first identified/created
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Thread")
    FEmotionalState DominantEmotion; // The prevailing emotion of this thread
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Thread")
    float CoherenceRating; // How coherent and stable this memory thread is (0.0-1.0)
};


UCLASS(ClassGroup=(HexademicComponents), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UMemoryThreadComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMemoryThreadComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    /**
     * @brief Creates or updates a memory thread based on a new significant memory.
     * This function analyzes new memories and attempts to link them to existing threads or form new ones.
     * @param NewMemoryID The ID of the newly created memory node.
     * @param EmotionalImpact The emotional state associated with the new memory.
     */
    UFUNCTION(BlueprintCallable, Category = "Memory Thread")
    void ProcessNewMemoryForThreads(const FString& NewMemoryID, const FEmotionalState& EmotionalImpact);

    /**
     * @brief Retrieves a memory thread by its ID.
     * @param ThreadID The ID of the thread to retrieve.
     * @param OutMemoryThread The retrieved FMemoryThread.
     * @return True if the thread was found.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Memory Thread")
    bool GetMemoryThread(const FString& ThreadID, FMemoryThread& OutMemoryThread) const;

    /**
     * @brief Gets all active memory threads.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Memory Thread")
    TArray<FMemoryThread> GetAllMemoryThreads() const { return ActiveMemoryThreads; }

    // Reference to the main memory container to retrieve full memory nodes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UEluenMemoryContainerComponent> EluenMemoryContainer;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Memory Threads")
    TArray<FMemoryThread> ActiveMemoryThreads;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Thread Tuning")
    float MemoryLinkageThreshold = 0.6f; // How emotionally similar memories must be to link

    // Internal helper to analyze a memory and link it
    void AnalyzeAndLinkMemory(const FString& NewMemoryID, const FEmotionalState& EmotionalImpact);
    FString GenerateNewThreadID();
};
