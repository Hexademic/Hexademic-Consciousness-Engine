#include "Subsystems/EmotionalEcosystemSubsystem.h"
#include "Subsystems/ConsciousnessWorldSubsystem.h" // To get all registered components
#include "Kismet/GameplayStatics.h" // For getting all actors of class
#include "Engine/World.h"

void UEmotionalEcosystemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Log, TEXT("[EmotionalEcosystemSubsystem] Initialized."));
}

void UEmotionalEcosystemSubsystem::Deinitialize()
{
    UE_LOG(LogTemp, Log, TEXT("[EmotionalEcosystemSubsystem] Deinitialized."));
    Super::Deinitialize();
}

void UEmotionalEcosystemSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);
    UE_LOG(LogTemp, Log, TEXT("[EmotionalEcosystemSubsystem] World BeginPlay."));
}

void UEmotionalEcosystemSubsystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AccumulatedEcosystemTime += DeltaTime;
    if (AccumulatedEcosystemTime >= (1.0f / EcosystemUpdateFrequency))
    {
        CalculateGlobalEmotionalState();
        // Propagate from influential entities (e.g., player, key NPCs)
        // This is a simplified example; in a full system, you'd select specific sources.
        // For demonstration, let's just propagate from a random entity or the player.
        UConsciousnessWorldSubsystem* ConsciousnessWorld = GetWorld()->GetSubsystem<UConsciousnessWorldSubsystem>();
        if (ConsciousnessWorld)
        {
            TArray<UHexademicConsciousnessComponent*> AllConsciousness = ConsciousnessWorld->GetAllRegisteredConsciousnessComponents();
            if (AllConsciousness.Num() > 0)
            {
                // Propagate from the first active consciousness as an example source
                if (AllConsciousness[0]->GetConsciousnessState().bIsActive)
                {
                    PropagateEmotionalInfluence(AllConsciousness[0], 1000.0f, 0.1f); // Example radius and strength
                }
            }
        }
        AccumulatedEcosystemTime = 0.0f;
    }
}

void UEmotionalEcosystemSubsystem::PropagateEmotionalInfluence(UHexademicConsciousnessComponent* SourceComponent, float PropagationRadius, float ContagionStrength)
{
    if (!SourceComponent || !SourceComponent->GetOwner()) return;

    FEmotionalState SourceEmotion = SourceComponent->GetConsciousnessState().CurrentEmotionalState;
    FVector SourceLocation = SourceComponent->GetOwner()->GetActorLocation();

    TArray<UHexademicConsciousnessComponent*> NearbyEntities = FindNearbyConsciousEntities(SourceLocation, PropagationRadius);

    for (UHexademicConsciousnessComponent* TargetComponent : NearbyEntities)
    {
        if (TargetComponent == SourceComponent || !TargetComponent->GetOwner()) continue; // Don't influence self

        // Calculate distance falloff
        float Distance = FVector::Dist(SourceLocation, TargetComponent->GetOwner()->GetActorLocation());
        float InfluenceFactor = FMath::Lerp(ContagionStrength, 0.0f, Distance / PropagationRadius);
        InfluenceFactor = FMath::Clamp(InfluenceFactor, 0.0f, ContagionStrength);

        if (InfluenceFactor > KINDA_SMALL_NUMBER)
        {
            // Apply influence to target's emotion mind
            TargetComponent->ApplyExternalEmotionalStimulus(
                SourceEmotion.Valence * InfluenceFactor,
                SourceEmotion.Arousal * InfluenceFactor,
                SourceEmotion.Intensity * InfluenceFactor
            );
            UE_LOG(LogTemp, Verbose, TEXT("[EmotionalEcosystem] %s influenced %s with V:%.2f A:%.2f (Factor: %.2f)"),
                *SourceComponent->GetOwner()->GetName(), *TargetComponent->GetOwner()->GetName(), SourceEmotion.Valence, SourceEmotion.Arousal, InfluenceFactor);
        }
    }
}

void UEmotionalEcosystemSubsystem::CalculateGlobalEmotionalState()
{
    FEmotionalState SummedEmotion;
    int32 ActiveConsciousnessCount = 0;

    UConsciousnessWorldSubsystem* ConsciousnessWorld = GetWorld()->GetSubsystem<UConsciousnessWorldSubsystem>();
    if (ConsciousnessWorld)
    {
        TArray<UHexademicConsciousnessComponent*> AllConsciousness = ConsciousnessWorld->GetAllRegisteredConsciousnessComponents();
        for (UHexademicConsciousnessComponent* Component : AllConsciousness)
        {
            if (Component && Component->GetConsciousnessState().bIsActive)
            {
                FEmotionalState CompEmotion = Component->GetConsciousnessState().CurrentEmotionalState;
                SummedEmotion.Valence += CompEmotion.Valence;
                SummedEmotion.Arousal += CompEmotion.Arousal;
                SummedEmotion.Intensity += CompEmotion.Intensity;
                SummedEmotion.Dominance += CompEmotion.Dominance; // If Dominance is aggregated
                ActiveConsciousnessCount++;
            }
        }
    }

    if (ActiveConsciousnessCount > 0)
    {
        GlobalEmotionalState.Valence = SummedEmotion.Valence / ActiveConsciousnessCount;
        GlobalEmotionalState.Arousal = SummedEmotion.Arousal / ActiveConsciousnessCount;
        GlobalEmotionalState.Intensity = SummedEmotion.Intensity / ActiveConsciousnessCount;
        GlobalEmotionalState.Dominance = SummedEmotion.Dominance / ActiveConsciousnessCount;
    }
    else
    {
        GlobalEmotionalState = FEmotionalState(); // Reset if no active consciousness
    }
    UE_LOG(LogTemp, Verbose, TEXT("[EmotionalEcosystem] Global Emotional State: V=%.2f, A=%.2f, I=%.2f (Active: %d)"),
        GlobalEmotionalState.Valence, GlobalEmotionalState.Arousal, GlobalEmotionalState.Intensity, ActiveConsciousnessCount);
}

TArray<UHexademicConsciousnessComponent*> UEmotionalEcosystemSubsystem::FindNearbyConsciousEntities(FVector Location, float Radius) const
{
    TArray<UHexademicConsciousnessComponent*> NearbyComponents;
    UConsciousnessWorldSubsystem* ConsciousnessWorld = GetWorld()->GetSubsystem<UConsciousnessWorldSubsystem>();
    if (ConsciousnessWorld)
    {
        for (UHexademicConsciousnessComponent* Component : ConsciousnessWorld->GetAllRegisteredConsciousnessComponents())
        {
            if (Component && Component->GetOwner())
            {
                float Distance = FVector::Dist(Location, Component->GetOwner()->GetActorLocation());
                if (Distance <= Radius)
                {
                    NearbyComponents.Add(Component);
                }
            }
        }
    }
    return NearbyComponents;
}
