#include "Mind/Memory/EluenMemoryContainerComponent.h"

UEluenMemoryContainerComponent::UEluenMemoryContainerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UEluenMemoryContainerComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UEluenMemoryContainerComponent::StoreMemory(const FString& MemoryID, const FString& MemoryContext)
{
    StoredMemories.Add(MemoryID, MemoryContext);
    UE_LOG(LogTemp, Log, TEXT("[MemoryContainer] Stored memory: %s"), *MemoryID);
}

bool UEluenMemoryContainerComponent::RecallMemory(const FString& MemoryID, FString& OutMemoryContext) const
{
    if (StoredMemories.Contains(MemoryID))
    {
        OutMemoryContext = StoredMemories[MemoryID];
        UE_LOG(LogTemp, Log, TEXT("[MemoryContainer] Recalled memory: %s"), *MemoryID);
        return true;
    }
    UE_LOG(LogTemp, Warning, TEXT("[MemoryContainer] Memory not found: %s"), *MemoryID);
    return false;
}

void UEluenMemoryContainerComponent::ForgetMemory(const FString& MemoryID)
{
    if (StoredMemories.Remove(MemoryID))
    {
        UE_LOG(LogTemp, Log, TEXT("[MemoryContainer] Forgot memory: %s"), *MemoryID);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[MemoryContainer] Memory not found for forgetting: %s"), *MemoryID);
    }
}
