#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mind/RecursiveAwarenessComponent.generated.h"

// Forward Declaration for UEluenMemoryContainerComponent if needed for direct memory access
class UEluenMemoryContainerComponent;
// Forward Declaration for FCognitiveMemoryNode if accessed directly
struct FCognitiveMemoryNode;

// FMemoryLineageBranch: A conceptual struct to represent a branch in memory's lineage
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FMemoryLineageBranch
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Lineage")
    FString OriginMemoryID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Lineage")
    TArray<FString> AssociatedMemories;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Lineage")
    float CumulativeValenceShift = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Lineage")
    float CumulativeArousalLift = 0.0f;
};

UCLASS(ClassGroup=(HexademicMind), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API URecursiveAwarenessComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URecursiveAwarenessComponent();

protected:
    virtual void BeginPlay() override;

public:
    /**
     * @brief Elects certain memory filaments as "Identity Roots" based on their significance.
     * This simulates the formation of core personality traits or deep-seated memories.
     */
    UFUNCTION(BlueprintCallable, Category = "Recursive Awareness")
    void ElectFilamentIdentityRoots();

    // The identified core identity roots
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Identity Roots")
    TArray<FString> IdentityRootFilaments;

    // Reference to the Memory Container (set by orchestrator)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UEluenMemoryContainerComponent> LinkedMemoryContainer;

protected:
    // Conceptual helper to get all memory branches (would involve complex memory graph traversal)
    TArray<FMemoryLineageBranch> GetAllBranches() const;
};
