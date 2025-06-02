#include "Subsystems/ConsciousnessWorldSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

void UConsciousnessWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Log, TEXT("[ConsciousnessWorldSubsystem] Initialized."));
}

void UConsciousnessWorldSubsystem::Deinitialize()
{
    UE_LOG(LogTemp, Log, TEXT("[ConsciousnessWorldSubsystem] Deinitialized."));
    RegisteredConsciousnessComponents.Empty(); // Clear all references
    Super::Deinitialize();
}

void UConsciousnessWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);
    UE_LOG(LogTemp, Log, TEXT("[ConsciousnessWorldSubsystem] World BeginPlay."));

    // Cache player pawn reference for LOD calculations
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        PlayerPawn = PlayerController->GetPawn();
    }

    // Start ticking if needed, or rely on external calls to UpdateAllConsciousnessLODs
    // GetWorld()->GetTimerManager().SetTimer(LODUpdateTimerHandle, this, &UConsciousnessWorldSubsystem::UpdateAllConsciousnessLODs, 1.0f, true);
}

void UConsciousnessWorldSubsystem::Tick(float DeltaTime)
{
    // If this subsystem is ticking, you could call UpdateAllConsciousnessLODs here periodically.
    // Or, this can be triggered by a game mode or a manager.
    // For now, let's keep it event-driven via UpdateAllConsciousnessLODs callable.
}

void UConsciousnessWorldSubsystem::RegisterConsciousnessComponent(UHexademicConsciousnessComponent* Component)
{
    if (Component && !RegisteredConsciousnessComponents.Contains(Component))
    {
        RegisteredConsciousnessComponents.Add(Component);
        UE_LOG(LogTemp, Log, TEXT("[ConsciousnessWorldSubsystem] Registered consciousness for %s. Total: %d"), *Component->GetOwner()->GetName(), RegisteredConsciousnessComponents.Num());
    }
}

void UConsciousnessWorldSubsystem::UnregisterConsciousnessComponent(UHexademicConsciousnessComponent* Component)
{
    if (Component)
    {
        RegisteredConsciousnessComponents.Remove(Component);
        UE_LOG(LogTemp, Log, TEXT("[ConsciousnessWorldSubsystem] Unregistered consciousness for %s. Total: %d"), *Component->GetOwner()->GetName(), RegisteredConsciousnessComponents.Num());
    }
}

void UConsciousnessWorldSubsystem::SetGlobalLODStrategy(FString NewStrategy)
{
    // Implement logic to switch between different LOD strategies (e.g., distance, importance, focus)
    UE_LOG(LogTemp, Log, TEXT("[ConsciousnessWorldSubsystem] Global LOD Strategy set to: %s"), *NewStrategy);
    // For now, we only implement distance-based.
}

void UConsciousnessWorldSubsystem::UpdateAllConsciousnessLODs()
{
    if (!PlayerPawn)
    {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            PlayerPawn = PlayerController->GetPawn();
        }
        if (!PlayerPawn) return; // Cannot do distance-based LOD without a player
    }

    for (UHexademicConsciousnessComponent* Component : RegisteredConsciousnessComponents)
    {
        if (Component && Component->IsValidLowLevelFast() && Component->GetOwner())
        {
            EConsciousnessLOD NewLOD = CalculateLODForComponent(Component);
            Component->SetConsciousnessLOD(NewLOD);
        }
    }
    UE_LOG(LogTemp, Verbose, TEXT("[ConsciousnessWorldSubsystem] Updated LODs for %d components."), RegisteredConsciousnessComponents.Num());
}

EConsciousnessLOD UConsciousnessWorldSubsystem::CalculateLODForComponent(UHexademicConsciousnessComponent* Component) const
{
    if (!PlayerPawn || !Component || !Component->GetOwner())
    {
        return EConsciousnessLOD::Dormant; // Cannot calculate, default to dormant
    }

    float DistanceToPlayer = FVector::Dist(PlayerPawn->GetActorLocation(), Component->GetOwner()->GetActorLocation());

    if (DistanceToPlayer <= HighLODDistanceThreshold)
    {
        return EConsciousnessLOD::Full;
    }
    else if (DistanceToPlayer <= MediumLODDistanceThreshold)
    {
        return EConsciousnessLOD::Reduced;
    }
    else
    {
        return EConsciousnessLOD::Minimal; // Or Dormant if very far away
    }
}
