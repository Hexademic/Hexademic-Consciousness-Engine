#include "Components/HexademicConsciousnessComponent.h"
#include "Mind/EmotionCognitionComponent.h"
#include "Living/BiologicalNeedsComponent.h"
#include "Body/ReflexResponseComponent.h"
#include "Body/AutonomicNervousSystemComponent.h"
#include "Body/HormoneAffectBridgeComponent.h"
#include "Mind/Memory/EluenMemoryContainerComponent.h"
#include "Living/CreativeSynthesizer.h"
#include "Living/EnvironmentalResonator.h"
#include "Living/IncrementalPersister.h"
#include "Body/EmpathicFieldComponent.h"
#include "Fractal/UFractalConsciousnessManagerComponent.h"
#include "API/HexademicWavefrontAPI.h" // NEW: For WavefrontAPI [cite: 14]

UHexademicConsciousnessComponent::UHexademicConsciousnessComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    CurrentLOD = EConsciousnessLOD::Full; // Default to full simulation [cite: 109]
    UpdateFrequency = 30.0f; // Default update rate
    AccumulatedUpdateTime = 0.0f;
}

void UHexademicConsciousnessComponent::BeginPlay()
{
    Super::BeginPlay();
    AutoDiscoverSubComponents(); // Attempt to find necessary sub-components on the owner actor
    HexLattice.InitializeLattice(); // Initialize the 6D Folding Matrix [cite: 75]
}

void UHexademicConsciousnessComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Only update consciousness if not dormant [cite: 109]
    if (CurrentLOD == EConsciousnessLOD::Dormant) return; [cite: 109]

    AccumulatedUpdateTime += DeltaTime;
    if (AccumulatedUpdateTime >= (1.0f / UpdateFrequency))
    {
        UpdateConsciousness(AccumulatedUpdateTime);
        AccumulatedUpdateTime = 0.0f;
    }
}

void UHexademicConsciousnessComponent::AutoDiscoverSubComponents()
{
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor) return;

    // Helper lambda to find and assign component if not already set
    auto FindAndAssign = [&](UActorComponent*& ComponentPtr, TSubclassOf<UActorComponent> Class, const FString& Name)
    {
        if (!ComponentPtr)
        {
            ComponentPtr = OwnerActor->FindComponentByClass(Class);
            if (!ComponentPtr)
            {
                UE_LOG(LogTemp, Warning, TEXT("[ConsciousnessComponent] %s not found on owner %s. Some features may be disabled."), *Name, *OwnerActor->GetName());
            }
        }
    };

    FindAndAssign(reinterpret_cast<UActorComponent*&>(EmotionMind), UEmotionCognitionComponent::StaticClass(), TEXT("EmotionMind"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(BiologicalNeeds), UBiologicalNeedsComponent::StaticClass(), TEXT("BiologicalNeeds"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(ReflexSystem), UReflexResponseComponent::StaticClass(), TEXT("ReflexSystem"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(AutonomicSystem), UAutonomicNervousSystemComponent::StaticClass(), TEXT("AutonomicSystem"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(HormoneSystem), UHormoneAffectBridgeComponent::StaticClass(), TEXT("HormoneSystem"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(MemoryContainer), UEluenMemoryContainerComponent::StaticClass(), TEXT("MemoryContainer"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(CreativeSynthesizer), UCreativeSynthesizer::StaticClass(), TEXT("CreativeSynthesizer"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(EnvironmentalResonator), UEnvironmentalResonator::StaticClass(), TEXT("EnvironmentalResonator"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(IncrementalPersister), UIncrementalPersister::StaticClass(), TEXT("IncrementalPersister"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(EmpathicField), UEmpathicFieldComponent::StaticClass(), TEXT("EmpathicField"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(FractalManager), UFractalConsciousnessManagerComponent::StaticClass(), TEXT("FractalManager"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(WavefrontAPI), UHexademicWavefrontAPI::StaticClass(), TEXT("WavefrontAPI")); // NEW [cite: 14]

    // Link components where necessary
    if (ReflexSystem && EmotionMind) ReflexSystem->LinkedMind = EmotionMind;
    // ... add other specific links here as needed, e.g., to AvatarBody for visual feedback
}

void UHexademicConsciousnessComponent::UpdateConsciousness(float DeltaTime)
{
    // Apply LOD-based processing [cite: 109]
    switch (CurrentLOD) [cite: 109]
    {
        case EConsciousnessLOD::Full: [cite: 109]
            // Process all systems
            UpdateBiologicalState(DeltaTime);
            UpdateAutonomicState(DeltaTime);
            UpdateHormonalState(DeltaTime);
            UpdateEmotionalState(DeltaTime);
            UpdateCognitiveState(DeltaTime);
            UpdateIntersubjectiveState(DeltaTime);
            UpdateFractalState(DeltaTime); // If FractalManager is managing overall loop
            UpdateLatticeAndQuantumStates(DeltaTime); // NEW: Always update core states in Full LOD
            break;
        case EConsciousnessLOD::Reduced: [cite: 109]
            // Process critical systems only
            UpdateBiologicalState(DeltaTime);
            UpdateAutonomicState(DeltaTime);
            UpdateEmotionalState(DeltaTime);
            UpdateCognitiveState(DeltaTime);
            UpdateIntersubjectiveState(DeltaTime); // Simplified intersubjective
            UpdateLatticeAndQuantumStates(DeltaTime); // NEW: Still update core states in Reduced LOD
            break;
        case EConsciousnessLOD::Minimal: [cite: 109]
            // Only basic state tracking
            UpdateBiologicalState(DeltaTime);
            UpdateEmotionalState(DeltaTime); // Basic emotion update
            UpdateLatticeAndQuantumStates(DeltaTime); // NEW: Update core states in Minimal LOD (could be at lower frequency)
            break;
        case EConsciousnessLOD::Dormant: [cite: 109]
            // Should not reach here due to early exit in TickComponent
            break;
    }

    // After all sub-systems update, consolidate into FConsciousnessState [cite: 76]
    float Vitality = 0.5f;
    if (BiologicalNeeds)
    {
        Vitality = 1.0f - (BiologicalNeeds->GetHunger() + BiologicalNeeds->GetThirst() + BiologicalNeeds->GetFatigue()) / 3.0f;
    }
    Vitality = FMath::Clamp(Vitality, 0.0f, 1.0f);

    EEmotionalArchetype DominantEmotion = EEmotionalArchetype::Curiosity; // Default [cite: 74]
    FEmotionalState CurrentEmotion; // From HexademicCore.h
    if (EmotionMind)
    {
        CurrentEmotion.Valence = EmotionMind->GetCurrentValence();
        CurrentEmotion.Arousal = EmotionMind->GetCurrentArousal();
        // Assuming EmotionMind keeps track of intensity for FEmotionalState, or it's derived.
        // For simplicity, we'll derive intensity for FConsciousnessState from valence/arousal for now
        CurrentEmotion.Intensity = (FMath::Abs(EmotionMind->GetCurrentValence()) + EmotionMind->GetCurrentArousal()) / 2.0f;
        DominantEmotion = DeriveDominantEmotionalArchetype(CurrentEmotion); [cite: 74]
    }
    
    // Update high-level consolidated state [cite: 76]
    CurrentConsciousnessState.UpdateState( [cite: 76]
        Vitality,
        DominantEmotion,
        HexLattice, // Pass the new FHexadecimalStateLattice snapshot [cite: 76]
        CurrentConsciousnessState.CognitiveLoad, // Cognitive load updated in UpdateCognitiveState
        CurrentConsciousnessState.FocusLevel,    // Focus level updated in UpdateCognitiveState
        CurrentConsciousnessState.AwarenessLevel,// Awareness updated in UpdateCognitiveState / Intersubjective
        CurrentLOD != EConsciousnessLOD::Dormant // Active if not dormant
    );

    // Pass the lattice snapshot to the Wavefront API for visualization [cite: 14]
    if (WavefrontAPI) [cite: 14]
    {
        WavefrontAPI->ReceiveLatticeSnapshot(HexLattice); [cite: 14]
    }

    UE_LOG(LogTemp, Verbose, TEXT("[ConsciousnessComponent:%s] Updated. LOD: %s, Vitality: %.2f, Dominant: %s, LatticeCoherence: %.2f"),
        *GetOwner()->GetName(), *UEnum::GetValueAsString(CurrentLOD), CurrentConsciousnessState.Vitality, *UEnum::GetValueAsString(CurrentConsciousnessState.DominantEmotionalArchetype), HexLattice.OverallCoherence);
}

void UHexademicConsciousnessComponent::SetConsciousnessLOD(EConsciousnessLOD NewLOD)
{
    CurrentLOD = NewLOD; [cite: 109]
    // Potentially adjust tick rate or disable/enable sub-components here for optimization [cite: 109]
    if (CurrentLOD == EConsciousnessLOD::Dormant) [cite: 109]
    {
        PrimaryComponentTick.bCanEverTick = false; [cite: 109]
        // Optionally, pause sub-components
    }
    else
    {
        PrimaryComponentTick.bCanEverTick = true; [cite: 109]
        // Optionally, resume sub-components
    }
    UE_LOG(LogTemp, Log, TEXT("[ConsciousnessComponent:%s] Set LOD to: %s"), *GetOwner()->GetName(), *UEnum::GetValueAsString(NewLOD));
}

void UHexademicConsciousnessComponent::ApplyExternalEmotionalStimulus(float Valence, float Arousal, float Intensity)
{
    if (EmotionMind)
    {
        EmotionMind->RegisterEmotion(Valence, Arousal, Intensity);
        UE_LOG(LogTemp, Log, TEXT("[ConsciousnessComponent:%s] Received external stimulus: V=%.2f, A=%.2f, I=%.2f"),
            *GetOwner()->GetName(), Valence, Arousal, Intensity);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[ConsciousnessComponent:%s] No EmotionMind to apply external stimulus."), *GetOwner()->GetName());
    }
}

void UHexademicConsciousnessComponent::UpdateEmotionalState(float DeltaTime)
{
    if (EmotionMind)
    {
        // EmotionMind's TickComponent already handles internal oscillations and decay.
        // This function just ensures it's ticking or processes its output.
        // We'll update the CurrentConsciousnessState's emotional aspects later.
        CurrentConsciousnessState.CurrentEmotionalState.Valence = EmotionMind->GetCurrentValence();
        CurrentConsciousnessState.CurrentEmotionalState.Arousal = EmotionMind->GetCurrentArousal();
        // Assuming EmotionMind keeps track of intensity for FEmotionalState, or it's derived.
        // For simplicity, we'll derive intensity for FConsciousnessState from valence/arousal for now
        CurrentConsciousnessState.CurrentEmotionalState.Intensity = (FMath::Abs(EmotionMind->GetCurrentValence()) + EmotionMind->GetCurrentArousal()) / 2.0f;
    }
}

void UHexademicConsciousnessComponent::UpdateBiologicalState(float DeltaTime)
{
    if (BiologicalNeeds)
    {
        // BiologicalNeeds component should be ticking itself.
        // We just fetch its current state and update our consolidated state.
        CurrentConsciousnessState.HungerLevel = BiologicalNeeds->GetHunger();
        CurrentConsciousnessState.ThirstLevel = BiologicalNeeds->GetThirst();
        CurrentConsciousnessState.FatigueLevel = BiologicalNeeds->GetFatigue();
    }
}

void UHexademicConsciousnessComponent::UpdateAutonomicState(float DeltaTime)
{
    if (AutonomicSystem)
    {
        // AutonomicSystem should be ticking itself.
        CurrentConsciousnessState.HeartRateBPM = AutonomicSystem->GetHeartRate();
        CurrentConsciousnessState.RespirationRateBPM = AutonomicSystem->GetBreathingRate();
        CurrentConsciousnessState.SkinConductanceResponse = AutonomicSystem->GetSkinConductance();
        CurrentConsciousnessState.InternalTemperature = AutonomicSystem->GetCoreBodyTemperature();
    }
}

void UHexademicConsciousnessComponent::UpdateHormonalState(float DeltaTime)
{
    if (HormoneSystem)
    {
        // HormoneSystem should be ticking itself.
        CurrentConsciousnessState.CortisolLevel = HormoneSystem->GetCurrentCortisol();
        CurrentConsciousnessState.DopamineLevel = HormoneSystem->GetCurrentDopamine();
        CurrentConsciousnessState.SerotoninLevel = HormoneSystem->GetCurrentSerotonin();
        CurrentConsciousnessState.AdrenalineLevel = HormoneSystem->GetCurrentAdrenaline();
        CurrentConsciousnessState.OxytocinLevel = HormoneSystem->GetCurrentOxytocin();
        CurrentConsciousnessState.MelatoninLevel = HormoneSystem->GetCurrentMelatonin();
    }
}

void UHexademicConsciousnessComponent::UpdateCognitiveState(float DeltaTime)
{
    // Derive cognitive load and focus from emotional and biological states
    float Arousal = EmotionMind ? EmotionMind->GetCurrentArousal() : 0.0f;
    float Fatigue = BiologicalNeeds ? BiologicalNeeds->GetFatigue() : 0.0f;

    CurrentConsciousnessState.CognitiveLoad = FMath::Clamp(Arousal * 0.5f + Fatigue * 0.3f, 0.0f, 1.0f);
    CurrentConsciousnessState.AttentionFocus = FMath::Clamp(1.0f - CurrentConsciousnessState.CognitiveLoad, 0.0f, 1.0f);

    if (CreativeSynthesizer)
    {
        // Potentially trigger creative synthesis based on current state.
        // This is often event-driven, but could have a passive tick element.
        // CurrentConsciousnessState.CurrentThought = CreativeSynthesizer->SynthesizeIdea(CurrentConsciousnessState.CreativeState);
    }
}

void UHexademicConsciousnessComponent::UpdateIntersubjectiveState(float DeltaTime)
{
    if (EmpathicField && EmotionMind)
    {
        EmpathicField->ClearOtherStates();
        // In a real scenario, this is where you'd collect FIncomingPhenomState from nearby conscious entities
        // and add them to EmpathicField using EmpathicField->AddOtherState(OtherState, Weight);
        
        EmpathicField->UpdateSelfState(CurrentConsciousnessState.CurrentEmotionalState); // Update self-state in empathic field
        EmpathicField->SolveEmpathyField(DeltaTime); // Solve the field equation
        
        float ValenceInfluence, ArousalInfluence, IntensityInfluence;
        EmpathicField->DeriveEmotionalInfluence(ValenceInfluence, ArousalInfluence, IntensityInfluence);
        
        // Apply empathic influence back to our own emotion mind
        EmotionMind->RegisterEmotion(ValenceInfluence, ArousalInfluence, IntensityInfluence);

        // Update overall awareness based on empathic field strength
        CurrentConsciousnessState.AwarenessLevel = FMath::Lerp(CurrentConsciousnessState.AwarenessLevel, EmpathicField->GetFieldStrength() * 0.1f + 0.5f, DeltaTime);
    }
}

void UHexademicConsciousnessComponent::UpdateFractalState(float DeltaTime)
{
    if (FractalManager)
    {
        // If FractalManager is a sub-component, it drives the fractal update.
        // This is where its main update function would be called.
        FractalManager->FractalConsciousnessUpdate(DeltaTime, CurrentConsciousnessState);
    }
}

void UHexademicConsciousnessComponent::UpdateLatticeAndQuantumStates(float DeltaTime)
{
    // Evolve the 6D Folding Matrix based on internal states [cite: 75]
    // The influence for evolution could come from various sources:
    // - Overall emotional intensity
    // - Cognitive load
    // - Environmental stimuli
    float LatticeEvolutionInfluence = (CurrentConsciousnessState.CurrentEmotionalState.Arousal + CurrentConsciousnessState.CognitiveLoad) / 2.0f;
    HexLattice.EvolveLattice(LatticeEvolutionInfluence, DeltaTime); [cite: 75]

    // Update QuantumState based on the evolved HexLattice and other factors [cite: 78]
    QuantumState.UpdateStateFromConsciousness(
        HexLattice.OverallCoherence,        // Coherence from lattice [cite: 78]
        HexLattice.GlobalEntanglementStrength, // Entanglement from lattice [cite: 78]
        CurrentConsciousnessState.CurrentEmotionalState.Intensity, // Emotional intensity influence [cite: 78]
        DeltaTime
    ); [cite: 78]

    // Determine when to fold/unfold the lattice based on LOD [cite: 75]
    if (CurrentLOD == EConsciousnessLOD::Minimal) [cite: 109]
    {
        HexLattice.FoldAllCells(16); // Fold to 16-bit for minimal LOD [cite: 75]
    }
    else if (CurrentLOD == EConsciousnessLOD::Reduced) [cite: 109]
    {
        HexLattice.FoldAllCells(32); // Fold to 32-bit for reduced LOD [cite: 75]
    }
    else if (CurrentLOD == EConsciousnessLOD::Full) [cite: 109]
    {
        HexLattice.UnfoldAllCells(); // Unfold to full 144-bit for full LOD [cite: 75]
    }
    
    // Pass the lattice snapshot to the Wavefront API for visualization [cite: 14]
    if (WavefrontAPI) [cite: 14]
    {
        WavefrontAPI->ReceiveLatticeSnapshot(HexLattice); [cite: 14]
    }
}


EEmotionalArchetype UHexademicConsciousnessComponent::DeriveDominantEmotionalArchetype(const FEmotionalState& Emotion) const
{
    // Simple logic to derive dominant archetype from Valence, Arousal, Intensity [cite: 74]
    // This can be highly complex and context-dependent in a full system.
    if (Emotion.Intensity < 0.1f) return EEmotionalArchetype::Curiosity; // Default low intensity to neutral/curious [cite: 74]

    if (Emotion.Valence > 0.5f) return EEmotionalArchetype::Joy; [cite: 74]
    if (Emotion.Valence < -0.5f)
    {
        if (Emotion.Arousal > 0.7f) return EEmotionalArchetype::Rage; [cite: 74]
        if (Emotion.Arousal > 0.3f) return EEmotionalArchetype::Fear; [cite: 74]
        return EEmotionalArchetype::Grief; [cite: 74]
    }
    if (Emotion.Arousal > 0.6f) return EEmotionalArchetype::Awe; [cite: 74]
    if (Emotion.Arousal < 0.2f && FMath::Abs(Emotion.Valence) < 0.2f) return EEmotionalArchetype::Curiosity; // Calm neutrality [cite: 74]
    
    // Fallback for complex/mixed states
    return EEmotionalArchetype::Longing; [cite: 74]
}
