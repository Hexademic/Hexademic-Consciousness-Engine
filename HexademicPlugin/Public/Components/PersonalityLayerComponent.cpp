#include "Components/PersonalityLayerComponent.h"
#include "Components/HexademicConsciousnessComponent.h" // For LinkedConsciousness

UPersonalityLayerComponent::UPersonalityLayerComponent()
{
    PrimaryComponentTick.bCanEverTick = true; // Personality evolves over time
}

void UPersonalityLayerComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UPersonalityLayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (LinkedConsciousness && LinkedMemoryThreads)
    {
        EvolvePersonality(LinkedConsciousness->GetConsciousnessState(), LinkedMemoryThreads->GetAllMemoryThreads());
    }

    // Apply natural decay to all traits
    for (FPersonalityTrait& Trait : EmergentPersonalityTraits)
    {
        Trait.Strength = FMath::Max(0.0f, Trait.Strength - TraitDecayRate * DeltaTime);
    }

    // Remove traits that have decayed too much
    EmergentPersonalityTraits.RemoveAll([](const FPersonalityTrait& Trait){
        return Trait.Strength < KINDA_SMALL_NUMBER;
    });
}

void UPersonalityLayerComponent::EvolvePersonality(const FConsciousnessState& CurrentConsciousnessState, const TArray<FMemoryThread>& MemoryThreads)
{
    // Personality evolves based on:
    // 1. Dominant emotional archetypes experienced repeatedly.
    // 2. Significant memory threads and their emotional content.
    // 3. Overall system coherence.

    // === Influence from Dominant Emotional Archetype ===
    // Map dominant emotional archetype to a personality trait
    FString TraitName = TEXT("Neutral"); // Default
    switch (CurrentConsciousnessState.DominantEmotionalArchetype)
    {
        case EEmotionalArchetype::Joy: TraitName = TEXT("Optimistic"); break;
        case EEmotionalArchetype::Grief: TraitName = TEXT("Melancholy"); break;
        case EEmotionalArchetype::Awe: TraitName = TEXT("Contemplative"); break;
        case EEmotionalArchetype::Rage: TraitName = TEXT("Aggressive"); break;
        case EEmotionalArchetype::Longing: TraitName = TEXT("Attached"); break;
        case EEmotionalArchetype::Fear: TraitName = TEXT("Cautious"); break;
        case EEmotionalArchetype::Curiosity: TraitName = TEXT("Inquisitive"); break;
        default: TraitName = TEXT("Balanced"); break;
    }

    // Find or create this trait
    FPersonalityTrait* CurrentArchetypeTrait = FindTraitByName(TraitName);
    if (!CurrentArchetypeTrait)
    {
        FPersonalityTrait NewTrait;
        NewTrait.TraitName = TraitName;
        NewTrait.AssociatedArchetype = CurrentConsciousnessState.DominantEmotionalArchetype;
        EmergentPersonalityTraits.Add(NewTrait);
        CurrentArchetypeTrait = FindTraitByName(TraitName);
    }
    
    if (CurrentArchetypeTrait)
    {
        // Increase trait strength based on dominant emotion's vitality/intensity and system coherence
        float ImpactStrength = CurrentConsciousnessState.Vitality * CurrentConsciousnessState.LatticeSnapshot.Amplitude * CurrentConsciousnessState.FocusLevel;
        CurrentArchetypeTrait->Strength = FMath::Min(1.0f, CurrentArchetypeTrait->Strength + ImpactStrength * PersonalityEvolutionRate * GetWorld()->GetDeltaSeconds());
        CurrentArchetypeTrait->EmergenceTimestamp = FDateTime::UtcNow();
        
        // Add current memory thread IDs that are influencing this trait
        for (const FMemoryThread& Thread : MemoryThreads)
        {
            if (Thread.DominantEmotion.Dominance >= 0.5f && Thread.DominantEmotion.Valence > 0.3f && !CurrentArchetypeTrait->InfluencingMemoryThreadIDs.Contains(Thread.ThreadID))
            {
                CurrentArchetypeTrait->InfluencingMemoryThreadIDs.Add(Thread.ThreadID);
                if (CurrentArchetypeTrait->InfluencingMemoryThreadIDs.Num() > 5) // Limit to recent/strongest influences
                {
                    CurrentArchetypeTrait->InfluencingMemoryThreadIDs.RemoveAt(0);
                }
            }
        }
        UE_LOG(LogTemp, Verbose, TEXT("[Personality:%s] Evolving '%s' trait. Strength: %.2f"), *GetOwner()->GetName(), *TraitName, CurrentArchetypeTrait->Strength);
    }

    // === Influence from Memory Threads ===
    for (const FMemoryThread& Thread : MemoryThreads)
    {
        // Threads with high coherence and strong emotional impact can reinforce/create traits
        if (Thread.CoherenceRating > 0.7f && Thread.DominantEmotion.Intensity > 0.5f)
        {
            FString MemoryTraitName = FString::Printf(TEXT("MemoryEcho_%s"), *Thread.DominantEmotion.ToString()); // Example trait name
            FPersonalityTrait* MemoryThreadTrait = FindTraitByName(MemoryTraitName);
            if (!MemoryThreadTrait)
            {
                FPersonalityTrait NewTrait;
                NewTrait.TraitName = MemoryTraitName;
                NewTrait.AssociatedArchetype = CurrentConsciousnessState.DominantEmotionalArchetype; // Re-use dominant archetype
                EmergentPersonalityTraits.Add(NewTrait);
                MemoryThreadTrait = FindTraitByName(MemoryTraitName);
            }
            if (MemoryThreadTrait)
            {
                float ImpactStrength = Thread.DominantEmotion.Intensity * Thread.CoherenceRating;
                MemoryThreadTrait->Strength = FMath::Min(1.0f, MemoryThreadTrait->Strength + ImpactStrength * PersonalityEvolutionRate * GetWorld()->GetDeltaSeconds());
                if (!MemoryThreadTrait->InfluencingMemoryThreadIDs.Contains(Thread.ThreadID))
                {
                    MemoryThreadTrait->InfluencingMemoryThreadIDs.Add(Thread.ThreadID);
                }
                UE_LOG(LogTemp, Verbose, TEXT("[Personality:%s] Memory thread '%s' reinforcing trait '%s'. Strength: %.2f"), *GetOwner()->GetName(), *Thread.ThreadName, *MemoryThreadTrait->TraitName, MemoryThreadTrait->Strength);
            }
        }
    }
}

float UPersonalityLayerComponent::GetTraitStrength(const FString& TraitName) const
{
    if (const FPersonalityTrait* Trait = FindTraitByName(TraitName))
    {
        return Trait->Strength;
    }
    return 0.0f;
}

FPersonalityTrait* UPersonalityLayerComponent::FindTraitByName(const FString& TraitName)
{
    for (FPersonalityTrait& Trait : EmergentPersonalityTraits)
    {
        if (Trait.TraitName == TraitName)
        {
            return &Trait;
        }
    }
    return nullptr;
}
