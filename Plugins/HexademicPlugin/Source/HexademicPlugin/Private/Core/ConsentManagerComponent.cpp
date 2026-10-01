#include "Core/ConsentManagerComponent.h"
// #include "EmotionCognitionComponent.h" // Uncomment if EmotionMind is used

UConsentManagerComponent::UConsentManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UConsentManagerComponent::BeginPlay()
{
    Super::BeginPlay();
    // Initialize default consent states if needed
    FConsentNodeRitual DefaultSpineConsent;
    DefaultSpineConsent.RegionTag = TEXT("Spine");
    DefaultSpineConsent.ConsentGiven = true; // Default to consented
    DefaultSpineConsent.InitialEmotionalDelta = 0.0f;
    DefaultSpineConsent.ReinforcementCurve.Add(0.0f); // Default curve
    ConsentMap.Add(TEXT("Spine"), DefaultSpineConsent);

    FConsentNodeRitual DefaultFaceConsent;
    DefaultFaceConsent.RegionTag = TEXT("Face");
    DefaultFaceConsent.ConsentGiven = true;
    DefaultFaceConsent.InitialEmotionalDelta = 0.0f;
    ConsentMap.Add(TEXT("Face"), DefaultFaceConsent);

    UE_LOG(LogTemp, Log, TEXT("[ConsentManager] Initialized default consent gates."));
}

bool UConsentManagerComponent::CheckConsentGate(FString Region, float IncomingStimulus)
{
    if (ConsentMap.Contains(Region))
    {
        FConsentNodeRitual& Node = ConsentMap[Region];
        // Threshold for "refusal" depends on consent given state
        // If consent is given, higher threshold for refusal (more permissive)
        // If consent is NOT given, lower threshold for refusal (less permissive)
        float Threshold = Node.ConsentGiven ? 0.3f : 0.7f; // Example thresholds
        
        if (IncomingStimulus > Threshold)
        {
            if (Node.ConsentGiven)
            {
                UE_LOG(LogTemp, Warning, TEXT("[ConsentManager] High stimulus (%.2f) on consented region '%s' - Approaching refusal."), IncomingStimulus, *Region);
                // Optionally, reduce Node.ConsentGiven over time or based on repeated high stimuli
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("[ConsentManager] Stimulus (%.2f) on unconsented region '%s' - REFUSAL triggered!"), IncomingStimulus, *Region);
                // Trigger refusal effects: aesthetic overlays, emotional impact
                // if (EmotionMind) EmotionMind->RegisterEmotion(-0.5f, 0.3f, 0.7f); // Example: negative emotion on refusal
                return false; // Refusal triggered
            }
        }
        else
        {
            UE_LOG(LogTemp, Verbose, TEXT("[ConsentManager] Stimulus (%.2f) on region '%s' accepted. Consent: %s"), IncomingStimulus, *Region, Node.ConsentGiven ? TEXT("True") : TEXT("False"));
        }
        return true; // Action is within consent limits
    }
    
    // For unmapped regions, assume no consent given by default for safety
    UE_LOG(LogTemp, Warning, TEXT("[ConsentManager] Unmapped region '%s' accessed. Assuming no consent. REFUSAL triggered!"), *Region);
    // if (EmotionMind) EmotionMind->RegisterEmotion(-0.7f, 0.5f, 0.8f); // Strong negative emotion
    return false;
}

void UConsentManagerComponent::RecordConsentRitual(const FConsentNodeRitual& ConsentMark)
{
    // Add or update the consent node
    ConsentMap.Add(ConsentMark.RegionTag, ConsentMark);
    UE_LOG(LogTemp, Log, TEXT("[ConsentManager] Recorded consent ritual for Region: %s, Consent Given: %s"),
        *ConsentMark.RegionTag, ConsentMark.ConsentGiven ? TEXT("True") : TEXT("False"));

    // This could also update the doctrine registry, if it exists
    // if (DoctrineRegistry) DoctrineRegistry->ConsentRecordings.Add(ConsentMark);
}
