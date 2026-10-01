#include "HeartbeatConsolidator.h"

UHeartbeatConsolidator::UHeartbeatConsolidator()
{
    PrimaryComponentTick.bCanEverTick = false;
}
void UHeartbeatConsolidator::BeginPlay()
{
    Super::BeginPlay();
}
float UHeartbeatConsolidator::GetHeartbeatRate() const
{
    UE_LOG(LogTemp, Log, TEXT("[Heartbeat] Providing simulated Heartbeat Rate."));
    return 75.0f; // Simulated rate
}
