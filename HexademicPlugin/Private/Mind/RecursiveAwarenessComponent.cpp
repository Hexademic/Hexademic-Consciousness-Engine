#include "Mind/RecursiveAwarenessComponent.h"
#include "Mind/Memory/EluenMemoryContainerComponent.h" // For LinkedMemoryContainer

URecursiveAwarenessComponent::URecursiveAwarenessComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void URecursiveAwarenessComponent::BeginPlay()
{
    Super::BeginPlay();
}

void URecursiveAwarenessComponent::ElectFilamentIdentityRoots()
{
    // Clear previous roots
    IdentityRootFilaments.Empty();

    TArray<FMemoryLineageBranch> AllBranches = GetAllBranches(); // Conceptual function

    for (FMemoryLineageBranch& Branch : AllBranches)
    {
        // Criteria for an identity root: significant emotional shifts and arousal lift
        if (Branch.CumulativeValenceShift > 0.8f && Branch.CumulativeArousalLift > 0.5f)
        {
            IdentityRootFilaments.Add(Branch.OriginMemoryID);
            UE_LOG(LogTemp, Log, TEXT("[RecursiveAwareness] Elected Identity Root: %s (ValenceShift: %.2f, ArousalLift: %.2f)"),
                *Branch.OriginMemoryID, Branch.CumulativeValenceShift, Branch.CumulativeArousalLift);
        }
    }
}

TArray<FMemoryLineageBranch> URecursiveAwarenessComponent::GetAllBranches() const
{
    TArray<FMemoryLineageBranch> Branches;
    // This is a highly conceptual function. In a real system, this would involve:
    // 1. Traversing UEluenMemoryContainerComponent's memories.
    // 2. Analyzing their connections, timestamps, and emotional content to form "branches".
    // For demonstration, we'll return a dummy branch.
    
    if (LinkedMemoryContainer)
    {
        // Example: Create a dummy branch from an existing memory
        if (LinkedMemoryContainer->StoredMemories.Num() > 0)
        {
            FMemoryLineageBranch DummyBranch;
            DummyBranch.OriginMemoryID = LinkedMemoryContainer->StoredMemories.CreateConstIterator()->Key; // First memory ID
            DummyBranch.CumulativeValenceShift = FMath::RandRange(0.0f, 1.0f);
            DummyBranch.CumulativeArousalLift = FMath::RandRange(0.0f, 1.0f);
            Branches.Add(DummyBranch);
            UE_LOG(LogTemp, Verbose, TEXT("[RecursiveAwareness] Generated dummy memory branch for testing."));
        }
    }
    return Branches;
}
