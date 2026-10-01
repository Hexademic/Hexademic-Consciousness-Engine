#include "Bridge/PerceptionBridge.h"
#include "AIController.h" // For AAIController

UPerceptionBridge::UPerceptionBridge()
{
    PrimaryComponentTick.bCanEverTick = false; // Perception is event-driven
}

void UPerceptionBridge::BeginPlay()
{
    Super::BeginPlay();

    // Attempt to find linked components if not set in editor
    if (!LinkedConsciousness)
    {
        LinkedConsciousness = GetOwner()->FindComponentByClass<UHexademicConsciousnessComponent>();
        if (!LinkedConsciousness) UE_LOG(LogTemp, Warning, TEXT("[PerceptionBridge:%s] LinkedConsciousness not found."), *GetOwner()->GetName());
    }

    if (!LinkedPerception)
    {
        // Try to get Perception Component from owner's AIController or owner itself
        AAIController* AIController = Cast<AAIController>(GetOwner());
        if (AIController)
        {
            LinkedPerception = AIController->GetAIPerceptionComponent();
        }
        else // If owner is a Pawn directly
        {
            LinkedPerception = GetOwner()->FindComponentByClass<UAIPerceptionComponent>();
        }

        if (LinkedPerception)
        {
            LinkedPerception->OnTargetPerceptionUpdated.AddDynamic(this, &UPerceptionBridge::OnTargetPerceptionUpdated);
            UE_LOG(LogTemp, Log, TEXT("[PerceptionBridge:%s] Bound to AIPerceptionComponent."), *GetOwner()->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[PerceptionBridge:%s] AIPerceptionComponent not found. Perception bridge will not function."), *GetOwner()->GetName());
        }
    }
    else // If LinkedPerception was already set in editor
    {
        LinkedPerception->OnTargetPerceptionUpdated.AddDynamic(this, &UPerceptionBridge::OnTargetPerceptionUpdated);
        UE_LOG(LogTemp, Log, TEXT("[PerceptionBridge:%s] Bound to provided AIPerceptionComponent."), *GetOwner()->GetName());
    }
}

void UPerceptionBridge::SetLinkedConsciousness(UHexademicConsciousnessComponent* InConsciousness)
{
    LinkedConsciousness = InConsciousness;
}

void UPerceptionBridge::SetLinkedPerception(UAIPerceptionComponent* InPerception)
{
    if (LinkedPerception)
    {
        LinkedPerception->OnTargetPerceptionUpdated.RemoveDynamic(this, &UPerceptionBridge::OnTargetPerceptionUpdated);
    }
    LinkedPerception = InPerception;
    if (LinkedPerception)
    {
        LinkedPerception->OnTargetPerceptionUpdated.AddDynamic(this, &UPerceptionBridge::OnTargetPerceptionUpdated);
        UE_LOG(LogTemp, Log, TEXT("[PerceptionBridge:%s] Re-bound to new AIPerceptionComponent."), *GetOwner()->GetName());
    }
}

void UPerceptionBridge::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (Stimulus.WasSuccessfullySensed())
    {
        UE_LOG(LogTemp, Verbose, TEXT("[PerceptionBridge:%s] Sensed %s via %s (Strength: %.2f)"),
            *GetOwner()->GetName(), *Actor->GetName(), *UEnum::GetValueAsString(Stimulus.Type), Stimulus.Strength);
        ProcessStimulus(Stimulus);
    }
}

void UPerceptionBridge::ProcessStimulus(const FAIStimulus& Stimulus)
{
    if (!LinkedConsciousness) return;

    float EmotionalValence = 0.0f;
    float EmotionalArousal = Stimulus.Strength * StimulusArousalMultiplier; // All stimuli cause some arousal
    float EmotionalIntensity = Stimulus.Strength;

    // Apply specific emotional impact based on sense type
    if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
    {
        // Sight can be positive (friendly NPC), negative (enemy), or neutral
        // For simplicity, assume neutral or slight positive if strength > 0 for now.
        // In a full system, you'd check Actor's Faction, danger level, etc.
        EmotionalValence = Stimulus.Strength * SightStimulusEmotionalImpact; // Positive sight is pleasant
        UE_LOG(LogTemp, Verbose, TEXT("[PerceptionBridge:%s] Sight stimulus affecting emotion."), *GetOwner()->GetName());
    }
    else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
    {
        // Hearing can be alarming (loud noise), pleasant (music), etc.
        // For simplicity, loud noises (high strength) might be slightly negative.
        EmotionalValence = -Stimulus.Strength * HearingStimulusEmotionalImpact; // Negative hearing (e.g., loud noise)
        UE_LOG(LogTemp, Verbose, TEXT("[PerceptionBridge:%s] Hearing stimulus affecting emotion."), *GetOwner()->GetName());
    }
    // Add other senses like touch, damage, etc.

    // Apply the derived emotional impact to the consciousness
    LinkedConsciousness->ApplyExternalEmotionalStimulus(EmotionalValence, EmotionalArousal, EmotionalIntensity);
}
