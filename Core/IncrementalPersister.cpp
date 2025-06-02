#include "IncrementalPersister.h"

UIncrementalPersister::UIncrementalPersister()
{
    PrimaryComponentTick.bCanEverTick = false;
}
void UIncrementalPersister::BeginPlay()
{
    Super::BeginPlay();
}
void UIncrementalPersister::PersistState(const FString& StateData)
{
    UE_LOG(LogTemp, Log, TEXT("[Persistence] Persisting State Data: %s"), *StateData.Left(100)); // Log first 100 chars
}
