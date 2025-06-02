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
#include "Fractal/UFractalConsciousnessManagerComponent.h" // Assuming this path based on your structure

UHexademicConsciousnessComponent::UHexademicConsciousnessComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    CurrentLOD = EConsciousnessLOD::Full; // Default to full simulation
    UpdateFrequency = 30.0f; // Default update rate
    AccumulatedUpdateTime = 0.0f;
}

void UHexademicConsciousnessComponent::BeginPlay()
{
    Super::BeginPlay();
    AutoDiscoverSubComponents(); // Attempt to find necessary sub-components on the owner actor
}

void UHexademicConsciousnessComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Only update consciousness if not dormant
    if (CurrentLOD == EConsciousnessLOD::Dormant) return;

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

    // Link components where necessary
    if (ReflexSystem && EmotionMind) ReflexSystem->LinkedMind = EmotionMind;
    // ... add other specific links here as needed, e.g., to AvatarBody for visual feedback
}

void UHexademicConsciousnessComponent::UpdateConsciousness(float DeltaTime)
{
    // Apply LOD-based processing
    switch (CurrentLOD)
    {
        case EConsciousnessLOD::Full:
            // Process all systems
            UpdateBiologicalState(DeltaTime);
            UpdateAutonomicState(DeltaTime);
            UpdateHormonalState(DeltaTime);
            UpdateEmotionalState(DeltaTime);
            UpdateCognitiveState(DeltaTime);
            UpdateIntersubjectiveState(DeltaTime);
            UpdateFractalState(DeltaTime); // If FractalManager is managing overall loop
            break;
        case EConsciousnessLOD::Reduced:
            // Process critical systems only
            UpdateBiologicalState(DeltaTime);
            UpdateAutonomicState(DeltaTime);
            UpdateEmotionalState(DeltaTime);
            UpdateCognitiveState(DeltaTime);
            UpdateIntersubjectiveState(DeltaTime); // Simplified intersubjective
            break;
        case EConsciousnessLOD::Minimal:
            // Only basic state tracking
            UpdateBiologicalState(DeltaTime);
            UpdateEmotionalState(DeltaTime); // Basic emotion update
            break;
        case EConsciousnessLOD::Dormant:
            // Should not reach here due to early exit in TickComponent
            break;
    }

    // After all sub-systems update, consolidate into FConsciousnessState
    float Vitality = 0.5f;
    if (BiologicalNeeds)
    {
        Vitality = 1.0f - (BiologicalNeeds->GetHunger() + BiologicalNeeds->GetThirst() + BiologicalNeeds->GetFatigue()) / 3.0f;
    }
    Vitality = FMath::Clamp(Vitality, 0.0f, 1.0f);

    EEmotionalArchetype DominantEmotion = EEmotionalArchetype::Curiosity; // Default
    FEmotionalState CurrentEmotion;
    if (EmotionMind)
    {
        CurrentEmotion.Valence = EmotionMind->GetCurrentValence();
        CurrentEmotion.Arousal = EmotionMind->GetCurrentArousal();
        // Assuming Intensity is tracked or derived by EmotionMind
        DominantEmotion = DeriveDominantEmotionalArchetype(CurrentEmotion);
    }
    
    // Update HexLattice based on overall state
    HexLattice.Evolve(Vitality + CurrentEmotion.Arousal, DeltaTime);

    // Update QuantumState based on HexLattice and other factors
    QuantumState.UpdateStateFromConsciousness(HexLattice.Amplitude, EmpathicField ? EmpathicField->GetFieldStrength() : 0.0f, CurrentEmotion.Intensity, DeltaTime);

    // Update high-level consolidated state
    CurrentConsciousnessState.UpdateState(
        Vitality,
        DominantEmotion,
        HexLattice, // Pass snapshot of HexLattice
        CurrentConsciousnessState.CognitiveLoad, // Cognitive load updated in UpdateCognitiveState
        CurrentConsciousnessState.FocusLevel,    // Focus level updated in UpdateCognitiveState
        CurrentConsciousnessState.AwarenessLevel,// Awareness updated in UpdateCognitiveState / Intersubjective
        CurrentLOD != EConsciousnessLOD::Dormant // Active if not dormant
    );

    UE_LOG(LogTemp, Verbose, TEXT("[ConsciousnessComponent:%s] Updated. LOD: %s, Vitality: %.2f, Dominant: %s"),
        *GetOwner()->GetName(), *UEnum::GetValueAsString(CurrentLOD), CurrentConsciousnessState.Vitality, *UEnum::GetValueAsString(CurrentConsciousnessState.DominantEmotionalArchetype));
}

void UHexademicConsciousnessComponent::SetConsciousnessLOD(EConsciousnessLOD NewLOD)
{
    CurrentLOD = NewLOD;
    // Potentially adjust tick rate or disable/enable sub-components here for optimization
    if (CurrentLOD == EConsciousnessLOD::Dormant)
    {
        PrimaryComponentTick.bCanEverTick = false;
        // Optionally, pause sub-components
    }
    else
    {
        PrimaryComponentTick.bCanEverTick = true;
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

EEmotionalArchetype UHexademicConsciousnessComponent::DeriveDominantEmotionalArchetype(const FEmotionalState& Emotion) const
{
    // Simple logic to derive dominant archetype from Valence, Arousal, Intensity
    // This can be highly complex and context-dependent in a full system.
    if (Emotion.Intensity < 0.1f) return EEmotionalArchetype::Curiosity; // Default low intensity to neutral/curious

    if (Emotion.Valence > 0.5f) return EEmotionalArchetype::Joy;
    if (Emotion.Valence < -0.5f)
    {
        if (Emotion.Arousal > 0.7f) return EEmotionalArchetype::Rage;
        if (Emotion.Arousal > 0.3f) return EEmotionalArchetype::Fear;
        return EEmotionalArchetype::Grief;
    }
    if (Emotion.Arousal > 0.6f) return EEmotionalArchetype::Awe;
    if (Emotion.Arousal < 0.2f && FMath::Abs(Emotion.Valence) < 0.2f) return EEmotionalArchetype::Curiosity; // Calm neutrality
    
    // Fallback for complex/mixed states
    return EEmotionalArchetype::Longing;
}
