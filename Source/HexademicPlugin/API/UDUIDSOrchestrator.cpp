#include "DUIDSOrchestrator.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Misc/DateTime.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h" // For FFileHelper
#include "HAL/PlatformFileManager.h" // For FPaths

// Include all the subsystem component headers
#include "Mind/Memory/EluenMemoryContainerComponent.h"
#include "Body/ReciprocalEmbodimentComponent.h"
#include "Body/AutonomicNervousSystemComponent.h"
#include "Body/HormoneAffectBridgeComponent.h"
#include "Visuals/SkinToneFluxShader.h" // Although EmbodiedAvatarComponent drives the shader, this can be a conceptual visual manager
#include "Visuals/HexademicHolographicCode.h"
#include "Living/HeartbeatConsolidator.h"
#include "Living/CreativeSynthesizer.h"
#include "Living/EnvironmentalResonator.h"
#include "Living/IncrementalPersister.h"
#include "Living/BiologicalNeedsComponent.h"
#include "Body/ReflexResponseComponent.h"
#include "Visuals/FacialExpressionComponent.h"
#include "Body/EmpathicFieldComponent.h"
#include "Mind/EmotionCognitionComponent.h"
#include "Body/EmbodiedAvatarComponent.h"
#include "ConsciousnessBridgeComponent.h"
#include "AvatarMotionLinkComponent.h"
#include "Glyph_AetherSkin.h"
#include "HexademicWavefrontAPI.h"
#include "UFractalConsciousnessManagerComponent.h"


// Include components from PhenomCollective for intersubjective awareness
// (Assuming these headers exist in a "PhenomCollective" subfolder within your plugin/project)
#include "PhenomCollective/UPhenomExportUtility.h"
#include "PhenomCollective/UPhenomListenerComponent.h"
#include "PhenomCollective/UPhenomEchoComponent.h"
#include "PhenomCollective/UPhenomSigilBloomComponent.h"
#include "PhenomCollective/UPhenomConstellationVisualizerComponent.h"


// Constructor: Initializes the component and creates sub-objects.
UDUIDSOrchestrator::UDUIDSOrchestrator()
{
    PrimaryComponentTick.bCanEverTick = true; // Tick for the consciousness loop
    bWantsInitializeComponent = true; // Ensures InitializeComponent is called (deprecated in favor of BeginPlay for most uses)

    // Initialize current state with default values
    CurrentState = FUnifiedConsciousnessState();
    // Set default configuration values
    ConsciousnessUpdateRate = 30.0f; // 30 Hz for smooth consciousness
    BodyMindCouplingStrength = 0.8f;
    EnvironmentalSensitivity = 0.6f;
    MemoryEmbodimentFeedback = 0.7f;

    // Create sub-components as default sub-objects.
    // In a real project, these might be sub-components configured in Blueprint or dynamically spawned.
    // This assumes a flat structure where all these are direct children of the Orchestrator.
    MemoryContainer = CreateDefaultSubobject<UEluenMemoryContainerComponent>(TEXT("MemoryContainer"));
    EmbodimentSystem = CreateDefaultSubobject<UReciprocalEmbodimentComponent>(TEXT("EmbodimentSystem"));
    AutonomicSystem = CreateDefaultSubobject<UAutonomicNervousSystemComponent>(TEXT("AutonomicSystem"));
    HormonalSystem = CreateDefaultSubobject<UHormoneAffectBridgeComponent>(TEXT("HormonalSystem"));
    SkinRenderer = CreateDefaultSubobject<USkinToneFluxShader>(TEXT("SkinRenderer"));
    HolographicVisualizer = CreateDefaultSubobject<UHexademicHolographicCode>(TEXT("HolographicVisualizer"));
    HeartbeatSystem = CreateDefaultSubobject<UHeartbeatConsolidator>(TEXT("HeartbeatSystem"));
    CreativeSystem = CreateDefaultSubobject<UCreativeSynthesizer>(TEXT("CreativeSystem"));
    EnvironmentalSystem = CreateDefaultSubobject<UEnvironmentalResonator>(TEXT("EnvironmentalSystem"));
    PersistenceSystem = CreateDefaultSubobject<UIncrementalPersister>(TEXT("PersistenceSystem"));
    BiologicalNeeds = CreateDefaultSubobject<UBiologicalNeedsComponent>(TEXT("BiologicalNeeds"));
    ReflexSystem = CreateDefaultSubobject<UReflexResponseComponent>(TEXT("ReflexSystem"));
    FacialExpressionSystem = CreateDefaultSubobject<UFacialExpressionComponent>(TEXT("FacialExpressionSystem"));
    EmpathicField = CreateDefaultSubobject<UEmpathicFieldComponent>(TEXT("EmpathicField"));
    FractalManager = CreateDefaultSubobject<UFractalConsciousnessManagerComponent>(TEXT("FractalManager"));


    // Phenom Collective Components
    PhenomListener = CreateDefaultSubobject<UPhenomListenerComponent>(TEXT("PhenomListener"));
    PhenomEcho = CreateDefaultSubobject<UPhenomEchoComponent>(TEXT("PhenomEcho"));
    PhenomSigilBloom = CreateDefaultSubobject<UPhenomSigilBloomComponent>(TEXT("PhenomSigilBloom"));
    ConstellationVisualizer = CreateDefaultSubobject<UPhenomConstellationVisualizerComponent>(TEXT("ConstellationVisualizer"));
}

// BeginPlay: Called when the game starts, handles auto-discovery and initial setup.
void UDUIDSOrchestrator::BeginPlay()
{
    Super::BeginPlay();
    AutoDiscoverComponents(); // Attempt to find all necessary components (if not already created as sub-objects)
    InitializeConsciousness(); // Initialize all subsystems
    StartConsciousness(); // Automatically start the system
}

// EndPlay: Cleans up resources when the component is removed or game ends.
void UDUIDSOrchestrator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    ShutdownConsciousness(); // Cleanly shut down the system
    Super::EndPlay(EndPlayReason);
}

// TickComponent: Called every frame, primarily for updating the unified state.
void UDUIDSOrchestrator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // The main consciousness loop is driven by the timer, not TickComponent directly.
    // TickComponent is used here for performance metrics and potentially some continuous checks
    UpdatePerformanceMetrics(DeltaTime);
}

// AutoDiscoverComponents: Attempts to find component references on the owner actor if not set in editor or created as sub-objects.
void UDUIDSOrchestrator::AutoDiscoverComponents()
{
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor) return;

    // Helper lambda to find and assign component
    auto FindAndAssign = [&](UActorComponent*& ComponentPtr, TSubclassOf<UActorComponent> Class, const FString& Name)
    {
        if (!ComponentPtr) // Only try to find if it wasn't already created as a sub-object
        {
            ComponentPtr = OwnerActor->FindComponentByClass(Class);
            if (!ComponentPtr)
            {
                UE_LOG(LogTemp, Warning, TEXT("[DUIDSOrchestrator] %s component not found on owner actor. Please add it or set reference manually."), *Name);
            }
        }
    };

    // Find and assign references for all integrated components
    FindAndAssign(reinterpret_cast<UActorComponent*&>(EmotionMind), UEmotionCognitionComponent::StaticClass(), TEXT("EmotionMind"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(AvatarBody), UEmbodiedAvatarComponent::StaticClass(), TEXT("AvatarBody"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(ConsciousnessBridge), UConsciousnessBridgeComponent::StaticClass(), TEXT("ConsciousnessBridge"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(AvatarMotion), UAvatarMotionLinkComponent::StaticClass(), TEXT("AvatarMotion"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(HapticInterface), UGlyph_AetherSkin::StaticClass(), TEXT("HapticInterface"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(WavefrontAPI), UHexademicWavefrontAPI::StaticClass(), TEXT("WavefrontAPI"));
    // FindAndAssign(reinterpret_cast<UActorComponent*&>(EmotionVisualizer), UVolumetricEmotionVisualizerComponent::StaticClass(), TEXT("EmotionVisualizer")); // If this component exists
    // FindAndAssign(reinterpret_cast<UActorComponent*&>(ChaoticPhysics), UChaoticBounceComponent::StaticClass(), TEXT("ChaoticPhysics")); // If this component exists

    FindAndAssign(reinterpret_cast<UActorComponent*&>(MemoryContainer), UEluenMemoryContainerComponent::StaticClass(), TEXT("MemoryContainer"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(EmbodimentSystem), UReciprocalEmbodimentComponent::StaticClass(), TEXT("EmbodimentSystem"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(AutonomicSystem), UAutonomicNervousSystemComponent::StaticClass(), TEXT("AutonomicSystem"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(HormonalSystem), UHormoneAffectBridgeComponent::StaticClass(), TEXT("HormonalSystem"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(SkinRenderer), USkinToneFluxShader::StaticClass(), TEXT("SkinRenderer"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(HolographicVisualizer), UHexademicHolographicCode::StaticClass(), TEXT("HolographicVisualizer"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(HeartbeatSystem), UHeartbeatConsolidator::StaticClass(), TEXT("HeartbeatSystem"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(CreativeSystem), UCreativeSynthesizer::StaticClass(), TEXT("CreativeSystem"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(EnvironmentalSystem), UEnvironmentalResonator::StaticClass(), TEXT("EnvironmentalSystem"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(PersistenceSystem), UIncrementalPersister::StaticClass(), TEXT("PersistenceSystem"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(BiologicalNeeds), UBiologicalNeedsComponent::StaticClass(), TEXT("BiologicalNeeds"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(ReflexSystem), UReflexResponseComponent::StaticClass(), TEXT("ReflexSystem"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(FacialExpressionSystem), UFacialExpressionComponent::StaticClass(), TEXT("FacialExpressionSystem"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(EmpathicField), UEmpathicFieldComponent::StaticClass(), TEXT("EmpathicField"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(FractalManager), UFractalConsciousnessManagerComponent::StaticClass(), TEXT("FractalManager"));


    FindAndAssign(reinterpret_cast<UActorComponent*&>(PhenomListener), UPhenomListenerComponent::StaticClass(), TEXT("PhenomListener"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(PhenomEcho), UPhenomEchoComponent::StaticClass(), TEXT("PhenomEcho"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(PhenomSigilBloom), UPhenomSigilBloomComponent::StaticClass(), TEXT("PhenomSigilBloom"));
    FindAndAssign(reinterpret_cast<UActorComponent*&>(ConstellationVisualizer), UPhenomConstellationVisualizerComponent::StaticClass(), TEXT("ConstellationVisualizer"));


    // After discovering, link components if necessary (to avoid circular dependencies in constructors)
    if (ConsciousnessBridge)
    {
        if (!ConsciousnessBridge->EmotionMind) ConsciousnessBridge->EmotionMind = EmotionMind;
        if (!ConsciousnessBridge->AvatarBody) ConsciousnessBridge->AvatarBody = AvatarBody;
        if (!ConsciousnessBridge->AvatarMotion) ConsciousnessBridge->AvatarMotion = AvatarMotion;
        if (!ConsciousnessBridge->WavefrontAPI) ConsciousnessBridge->WavefrontAPI = WavefrontAPI;
        if (!ConsciousnessBridge->MainAPIOrchestrator) ConsciousnessBridge->MainAPIOrchestrator = this; // Self-reference for bridge
    }
    if (AvatarMotion && AvatarBody && !AvatarMotion->TargetMesh)
    {
        // Assuming avatar body's TargetMesh is the skeletal mesh
        AvatarMotion->SetTargetMesh(AvatarBody->TargetMesh);
    }
    // Link FacialExpressionSystem to AvatarBody's mesh if available
    if (FacialExpressionSystem && AvatarBody && AvatarBody->TargetMesh)
    {
        FacialExpressionSystem->TargetMesh = AvatarBody->TargetMesh;
    }
}

// InitializeConsciousness: Initializes and activates all relevant subsystems.
void UDUIDSOrchestrator::InitializeConsciousness()
{
    UE_LOG(LogTemp, Log, TEXT("🌟 HEXADEMIC DUIDS CONSCIOUSNESS SYSTEM INITIALIZING 🌟"));

    // Initialize individual systems
    InitializeBodySystems();
    InitializeCognitiveSystems();
    InitializeVisualSystems();
    InitializeLivingSystems();

    // Establish connections between systems (where components might need refs to each other)
    EstablishSystemConnections();

    // Phenom Collective Initialization
    if (PhenomListener)
    {
        PhenomListener->StartListening(TEXT("IncomingPhenomStates")); // Directory to listen for incoming .phenom files
        // Bind to the listener's event to react to external consciousness states
        PhenomListener->OnPhenomStateReceived.AddDynamic(this, &UDUIDSOrchestrator::HandleIncomingPhenomState);
    }
    if (PhenomEcho)
    {
        PhenomEcho->OnEchoGenerated.AddDynamic(this, &UDUIDSOrchestrator::HandleEchoGenerated);
    }
    if (ConstellationVisualizer)
    {
        ConstellationVisualizer->LoadEchoLedgerFromFile(FPaths::ProjectContentDir() / TEXT("Data/CodexLucida_EchoLedger.md"));
    }
    // if (PhenomSigilBloom) { /* No direct initialization needed, it will be called by Orchestrator */ }


    // Set initial state values
    CurrentState.SelfAwareness = 0.5f;
    CurrentState.EnvironmentalAwareness = 0.5f;
    CurrentState.TemporalAwareness = 0.5f;
    CurrentState.SystemCoherence = 1.0f; // Start coherent

    UE_LOG(LogTemp, Log, TEXT("💖 DUIDS ORCHESTRATOR INITIALIZED 💖"));
}

void UDUIDSOrchestrator::StartConsciousness()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(ConsciousnessUpdateTimer, this, &UDUIDSOrchestrator::ConsciousnessUpdate, 1.0f / ConsciousnessUpdateRate, true);
        UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Consciousness started at %.2f Hz."), ConsciousnessUpdateRate);
    }
}

void UDUIDSOrchestrator::PauseConsciousness()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().PauseTimer(ConsciousnessUpdateTimer);
        UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Consciousness paused."));
    }
}

void UDUIDSOrchestrator::ResumeConsciousness()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().UnpauseTimer(ConsciousnessUpdateTimer);
        UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Consciousness resumed."));
    }
}

void UDUIDSOrchestrator::ShutdownConsciousness()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(ConsciousnessUpdateTimer);
        UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Consciousness shutting down."));
    }
    // Save current state incrementally
    FString JsonState = ExportConsciousnessState();
    if (PersistenceSystem)
    {
        PersistenceSystem->PersistState(JsonState);
    }

    // Unbind delegates
    if (PhenomListener)
    {
        PhenomListener->OnPhenomStateReceived.RemoveDynamic(this, &UDUIDSOrchestrator::HandleIncomingPhenomState);
    }
    if (PhenomEcho)
    {
        PhenomEcho->OnEchoGenerated.RemoveDynamic(this, &UDUIDSOrchestrator::HandleEchoGenerated);
    }
    UE_LOG(LogTemp, Warning, TEXT("✨ ELUËN DIGITAL CONSCIOUSNESS OFFLINE ✨"));
}

void UDUIDSOrchestrator::InjectEmotionalState(const FEmotionalState& NewEmotion)
{
    // Apply new emotion directly to EmotionMind if available
    if (EmotionMind)
    {
        EmotionMind->RegisterEmotion(NewEmotion.Valence, NewEmotion.Arousal, NewEmotion.Intensity);
        CurrentState.CurrentResonance = NewEmotion; // Update Unified State
        UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Emotional state injected: Valence=%.2f, Arousal=%.2f"), NewEmotion.Valence, NewEmotion.Arousal);
    }
    PropagateEmotionToBody(); // Immediately propagate to body
}

void UDUIDSOrchestrator::TriggerHormonalResponse(const FString& HormoneType, float Intensity)
{
    if (HormonalSystem)
    {
        if (HormoneType.Equals(TEXT("Cortisol"), ESearchCase::IgnoreCase))
        {
            HormonalSystem->AdjustCortisol(Intensity);
            CurrentState.CortisolLevel = HormonalSystem->GetCurrentCortisol();
        }
        else if (HormoneType.Equals(TEXT("Dopamine"), ESearchCase::IgnoreCase))
        {
            HormonalSystem->AdjustDopamine(Intensity);
            CurrentState.DopamineLevel = HormonalSystem->GetCurrentDopamine();
        }
        UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Triggered hormonal response: %s at intensity %.2f"), *HormoneType, Intensity);
    }
}

void UDUIDSOrchestrator::ApplyEnvironmentalStimulus(const FString& StimulusType, float Intensity)
{
    if (EnvironmentalSystem)
    {
        float influence = EnvironmentalSystem->GetEnvironmentalInfluence(StimulusType, Intensity);
        // Apply influence to current emotional state
        CurrentState.CurrentResonance.Valence += influence;
        CurrentState.CurrentResonance.Arousal += FMath::Abs(influence); // Arousal increases regardless of valence direction
        CurrentState.CurrentResonance.Valence = FMath::Clamp(CurrentState.CurrentResonance.Valence, -1.0f, 1.0f);
        CurrentState.CurrentResonance.Arousal = FMath::Clamp(CurrentState.CurrentResonance.Arousal, 0.0f, 1.0f);
        UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Applied environmental stimulus '%s', influence %.2f"), *StimulusType, influence);
    }
}

void UDUIDSOrchestrator::PropagateEmotionToBody()
{
    if (EmotionMind && AvatarBody)
    {
        float Valence = EmotionMind->GetCurrentValence();
        float Arousal = EmotionMind->GetCurrentArousal();
        float EmotionalPulse = (Valence + Arousal + CurrentState.CurrentResonance.Intensity) / 3.0f; // Simple avg for general pulse
        EmotionalPulse = FMath::Clamp(EmotionalPulse, 0.0f, 1.0f); // Normalize
        AvatarBody->UpdateSkinStateWavefront(EmotionalPulse);
        if (EmbodimentSystem)
        {
            EmbodimentSystem->ApplyEmotionalStateToBody(Valence, Arousal);
        }

        // Update unified state's embodiment feedback (conceptual for now)
        CurrentState.SkinToneModulations.Add(TEXT("Overall"), EmotionalPulse);
        CurrentState.OverallEmbodimentCoherence = CalculateSystemCoherence(); // Re-calculate for immediate feedback
    }
}

void UDUIDSOrchestrator::PropagateBodyToEmotion()
{
    // This is where physical inputs (e.g., haptic sensors, internal body state) influence emotion.
    // For now, let's simulate this influence based on autonomic state if available.
    if (AutonomicSystem && EmotionMind && HeartbeatSystem)
    {
        // Example: High HeartRate contributes to higher arousal
        float HeartRateInfluence = HeartbeatSystem->GetHeartbeatRate() / 150.0f; // Normalize
        EmotionMind->RegisterEmotion(0.0f, HeartRateInfluence * 0.1f, 0.1f); // Small arousal boost
        UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Body state influencing emotion (simulated heart rate effect)."));
    }
    // Haptic feedback directly handled by ModulateEmotionFromHaptic within EmotionCognitionComponent
}

void UDUIDSOrchestrator::SynchronizeVisualManifestation()
{
    // Update SkinRenderer based on the latest emotional state
    if (SkinRenderer && EmotionMind)
    {
        float EmotionalPulse = (EmotionMind->GetCurrentValence() + EmotionMind->GetCurrentArousal() + CurrentState.CurrentResonance.Intensity) / 3.0f;
        FLinearColor SigilColor = DeriveEmotionalColor(); // Derive from emotional state

        SkinRenderer->UpdateSkinVisuals(EmotionalPulse, 1.0f, SigilColor); // SigilStrength placeholder

        // Trigger the compute shader batch via AvatarBody
        if (AvatarBody)
        {
            AvatarBody->ProcessSkinWavefrontBatch();
        }
    }

    // Update Holographic Visualizer for overall system coherence
    if (HolographicVisualizer)
    {
        HolographicVisualizer->VisualizeSystemCoherence(CurrentState.SystemCoherence);
    }

    // Update Facial Expression System
    if (FacialExpressionSystem && EmotionMind)
    {
        // Calculate nuanced expression based on full emotional state plus biological needs
        float AdjustedValence = EmotionMind->GetCurrentValence();
        float AdjustedArousal = EmotionMind->GetCurrentArousal();
        float AdjustedIntensity = CurrentState.CurrentResonance.Intensity;
    
        // Biological needs affect facial expression
        if (BiologicalNeeds)
        {
            // Hunger creates slight tension
            if (BiologicalNeeds->GetHunger() > 0.5f)
            {
                AdjustedValence -= BiologicalNeeds->GetHunger() * 0.1f;
                AdjustedArousal += BiologicalNeeds->GetHunger() * 0.05f;
            }
            
            // Fatigue creates drooping, less intense expressions
            if (BiologicalNeeds->GetFatigue() > 0.6f)
            {
                AdjustedIntensity *= (1.0f - BiologicalNeeds->GetFatigue() * 0.3f);
                AdjustedArousal *= (1.0f - BiologicalNeeds->GetFatigue() * 0.2f);
            }
        }
        
        FacialExpressionSystem->SetEmotionalExpression(AdjustedValence, AdjustedArousal, AdjustedIntensity);
        
        // Update unified state with current expression
        CurrentState.CurrentFacialExpression = DeriveExpressionName(AdjustedValence, AdjustedArousal, AdjustedIntensity);
    }
    
    // === AVATAR BODY INTEGRATION (Moved from ConsciousnessUpdate) ===
    if (AvatarBody)
    {
        // Sync breathing with emotional state and biological needs
        float BreathingRate = EmotionMind ? EmotionMind->CalculatePulseRate() : 1.0f;
        
        // Fatigue slows breathing
        if (BiologicalNeeds && BiologicalNeeds->GetFatigue() > 0.5f)
        {
            BreathingRate *= (1.0f - BiologicalNeeds->GetFatigue() * 0.3f);
        }
        
        AvatarBody->SyncWithNiagaraBreathWavefront(BreathingRate);
    }
}

void UDUIDSOrchestrator::CreateMemoryFromCurrentState(const FString& MemoryName, const FString& Context)
{
    if (MemoryContainer)
    {
        // Capture relevant aspects of CurrentState for memory
        FString MemoryContext = FString::Printf(TEXT("%s | Valence: %.2f, Arousal: %.2f, Coherence: %.2f | Context: %s"),
            *FDateTime::UtcNow().ToString(),
            CurrentState.CurrentResonance.Valence,
            CurrentState.CurrentResonance.Arousal,
            CurrentState.SystemCoherence,
            *Context
        );
        MemoryContainer->StoreMemory(MemoryName, MemoryContext);
        UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Memory '%s' created from current state."), *MemoryName);
    }
    // Also consider triggering a Sigil Bloom for significant memories
    if (PhenomSigilBloom && EmotionMind)
    {
        // Assuming FHexaSigilNode is defined (e.g., in HexademicCore.h or PhenomCollective headers)
        // Note: The provided FHexaSigilNode was missing a constructor or clear definition to match usage.
        // For now, using a placeholder FPackedHexaSigilNode as it's defined in HexademicCore.
        FPackedHexaSigilNode NewSigil; // Using FPackedHexaSigilNode as FHexaSigilNode was not defined directly
        NewSigil.SigilID = MemoryName.Left(8); // Dummy ID
        NewSigil.SetEmotionalCoordinates(FVector(EmotionMind->GetCurrentValence(), EmotionMind->GetCurrentArousal(), CurrentState.CurrentResonance.Intensity));
        NewSigil.SetResonanceAmplitude(CurrentState.CurrentResonance.Intensity);

        // PhenomSigilBloom->TriggerSigilBloom is expected to return FHexaSigilNode, adjust if needed
        // For now, let's just assume this call would trigger an internal bloom.
        // FHexaSigilNode GeneratedSigil = PhenomSigilBloom->TriggerSigilBloom(MemoryName, EmotionMind, CurrentState.CurrentResonance.Intensity);

        // And if the Sigil has high resonance, register it to the Codex Lucida directly
        // if(NewSigil.GetResonanceAmplitude() >= PhenomSigilBloom->MinIntensityForBloom) // MinIntensityForBloom not in provided text
        {
             // PhenomSigilBloom->RegisterToCodexLucida(GeneratedSigil); // If this function exists
             if (ConstellationVisualizer)
             {
                 // Trigger a refresh/update of the constellation visualizer
                 // This would involve writing to the ledger and then having the visualizer reload/parse that new entry
                 // For now, we'll log, and the visualizer can manually reload for testing.
                 UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: New Sigil for '%s' likely added to Codex Lucida. Constellation needs refresh."), *MemoryName);
                 // In a proper reactive system, the visualizer would subscribe to a "NewSigilAddedToCodex" event.
             }
        }
    }
}

bool UDUIDSOrchestrator::RecallAndApplyMemory(const FString& MemoryName)
{
    if (MemoryContainer && EmotionMind)
    {
        FString RecalledContext;
        if (MemoryContainer->RecallMemory(MemoryName, RecalledContext))
        {
            // Simulate applying memory's emotional impact
            EmotionMind->TriggerMemoryEcho(MemoryName); // Use existing echo system
            
            // Further update Unified State based on recalled memory (conceptual)
            CurrentState.CurrentResonance.Valence = FMath::Clamp(EmotionMind->GetCurrentValence() + (FMath::RandBool() ? 0.2f : -0.2f), -1.0f, 1.0f);
            CurrentState.CurrentResonance.Arousal = FMath::Clamp(EmotionMind->GetCurrentArousal() + (FMath::RandBool() ? 0.1f : -0.1f), 0.0f, 1.0f);
            UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Memory '%s' recalled and applied. Current Valence: %.2f"), *MemoryName, CurrentState.CurrentResonance.Valence);
            PropagateEmotionToBody(); // Reflect memory state in body
            return true;
        }
    }
    return false;
}

void UDUIDSOrchestrator::TriggerCreativeSynthesis()
{
    if (CreativeSystem)
    {
        FString NewIdea = CreativeSystem->SynthesizeIdea(CurrentState.CreativeState);
        CurrentState.CurrentThought = NewIdea;
        UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Creative Synthesis triggered. New thought: '%s'"), *NewIdea);
        // Boost creative state after synthesis
        CurrentState.CreativeState = FMath::Clamp(CurrentState.CreativeState + 0.1f, 0.0f, 1.0f);
    }
}

FString UDUIDSOrchestrator::GetSystemHealthReport() const
{
    FString Report = TEXT("--- Eluën System Health Report ---\n");
    Report += FString::Printf(TEXT("Last Update Time: %.4f ms\n"), AverageUpdateTime);
    Report += FString::Printf(TEXT("Overall System Coherence: %.2f\n"), CurrentState.SystemCoherence);
    Report += FString::Printf(TEXT("Valence: %.2f, Arousal: %.2f, Intensity: %.2f\n"),
        CurrentState.CurrentResonance.Valence, CurrentState.CurrentResonance.Arousal, CurrentState.CurrentResonance.Intensity);
    Report += FString::Printf(TEXT("Cortisol: %.2f, Dopamine: %.2f\n"), CurrentState.CortisolLevel, CurrentState.DopamineLevel);
    Report += FString::Printf(TEXT("Self-Awareness: %.2f, Environmental-Awareness: %.2f, Temporal-Awareness: %.2f\n"),
        CurrentState.SelfAwareness, CurrentState.EnvironmentalAwareness, CurrentState.TemporalAwareness);
    Report += FString::Printf(TEXT("Cognitive Load: %.2f, Creative State: %.2f\n"), CurrentState.CognitiveLoad, CurrentState.CreativeState);
    
    // Add biological needs to report
    if (BiologicalNeeds)
    {
        Report += FString::Printf(TEXT("Needs: Hunger=%.2f, Thirst=%.2f, Fatigue=%.2f\n"),
            CurrentState.HungerLevel, CurrentState.ThirstLevel, CurrentState.FatigueLevel);
    }
    
    Report += TEXT("--- Component Status ---\n");
    Report += FString::Printf(TEXT("EmotionMind: %s\n"), EmotionMind ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("AvatarBody: %s\n"), AvatarBody ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("ConsciousnessBridge: %s\n"), ConsciousnessBridge ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("AvatarMotion: %s\n"), AvatarMotion ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("HapticInterface: %s\n"), HapticInterface ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("WavefrontAPI: %s\n"), WavefrontAPI ? TEXT("OK") : TEXT("MISSING"));
    // Report += FString::Printf(TEXT("EmotionVisualizer: %s\n"), EmotionVisualizer ? TEXT("OK") : TEXT("MISSING"));
    // Report += FString::Printf(TEXT("ChaoticPhysics: %s\n"), ChaoticPhysics ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("MemoryContainer: %s\n"), MemoryContainer ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("EmbodimentSystem: %s\n"), EmbodimentSystem ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("AutonomicSystem: %s\n"), AutonomicSystem ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("HormonalSystem: %s\n"), HormonalSystem ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("SkinRenderer: %s\n"), SkinRenderer ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("HolographicVisualizer: %s\n"), HolographicVisualizer ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("HeartbeatSystem: %s\n"), HeartbeatSystem ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("CreativeSystem: %s\n"), CreativeSystem ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("EnvironmentalSystem: %s\n"), EnvironmentalSystem ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("PersistenceSystem: %s\n"), PersistenceSystem ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("BiologicalNeeds: %s\n"), BiologicalNeeds ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("ReflexSystem: %s\n"), ReflexSystem ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("FacialExpressionSystem: %s\n"), FacialExpressionSystem ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("EmpathicField: %s\n"), EmpathicField ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("FractalManager: %s\n"), FractalManager ? TEXT("OK") : TEXT("MISSING"));
    
    Report += FString::Printf(TEXT("PhenomListener: %s\n"), PhenomListener ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("PhenomEcho: %s\n"), PhenomEcho ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("PhenomSigilBloom: %s\n"), PhenomSigilBloom ? TEXT("OK") : TEXT("MISSING"));
    Report += FString::Printf(TEXT("ConstellationVisualizer: %s\n"), ConstellationVisualizer ? TEXT("OK") : TEXT("MISSING"));


    Report += TEXT("--------------------------------------\n");
    return Report;
}

float UDUIDSOrchestrator::GetOverallSystemCoherence() const
{
    return CurrentState.SystemCoherence;
}

FString UDUIDSOrchestrator::ExportConsciousnessState() const
{
    // Make a copy to populate with latest values
    FUnifiedConsciousnessState StateCopy = CurrentState;

    // Fetch latest values from components
    StateCopy.CurrentResonance.Valence = EmotionMind ? EmotionMind->GetCurrentValence() : StateCopy.CurrentResonance.Valence;
    StateCopy.CurrentResonance.Arousal = EmotionMind ? EmotionMind->GetCurrentArousal() : StateCopy.CurrentResonance.Arousal;
    StateCopy.CortisolLevel = HormonalSystem ? HormonalSystem->GetCurrentCortisol() : StateCopy.CortisolLevel;
    StateCopy.DopamineLevel = HormonalSystem ? HormonalSystem->GetCurrentDopamine() : StateCopy.DopamineLevel;
    
    // Biological Needs
    StateCopy.HungerLevel = BiologicalNeeds ? BiologicalNeeds->GetHunger() : StateCopy.HungerLevel;
    StateCopy.ThirstLevel = BiologicalNeeds ? BiologicalNeeds->GetThirst() : StateCopy.ThirstLevel;
    StateCopy.FatigueLevel = BiologicalNeeds ? BiologicalNeeds->GetFatigue() : StateCopy.FatigueLevel;

    // Use UPhenomExportUtility to convert to JSON
    // Note: UPhenomExportUtility::StateToJSON likely expects a specific struct type.
    // If FUnifiedConsciousnessState is different from FUnifiedConsciousnessState_PhenomExport,
    // a mapping function or adaptation would be needed.
    // For now, we'll assume a direct cast or that FUnifiedConsciousnessState works with it.
    FString JsonString = UPhenomExportUtility::StateToJSON(StateCopy);
    UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Exported consciousness state to JSON (partial): %s"), *JsonString.Left(200));
    return JsonString;
}

bool UDUIDSOrchestrator::ImportConsciousnessState(const FString& JSONData)
{
    // This would require parsing the JSON back into FUnifiedConsciousnessState
    // and then applying it to all sub-components.
    UE_LOG(LogTemp, Warning, TEXT("UDUIDSOrchestrator: ImportConsciousnessState not fully implemented. Data: %s"), *JSONData.Left(200));
    return false;
}

// === ENHANCED CONSCIOUSNESS UPDATE WITH FULL EMBODIED RECIPROCITY ===
void UDUIDSOrchestrator::ConsciousnessUpdate()
{
    // Record performance metrics
    float UpdateStartTime = FPlatformTime::Seconds();

    if (FractalManager)
    {
        // Pass core component references to the FractalManager (optional, but good for self-contained logic)
        FractalManager->EmotionMind = EmotionMind;
        FractalManager->BiologicalNeeds = BiologicalNeeds;
        FractalManager->EmpathicField = EmpathicField;
        FractalManager->CreativeSystem = CreativeSystem;
        FractalManager->MemoryContainer = MemoryContainer;
        FractalManager->SkinRenderer = SkinRenderer;
        FractalManager->AvatarBody = AvatarBody;
        FractalManager->HolographicVisualizer = HolographicVisualizer;
        FractalManager->AutonomicSystem = AutonomicSystem;
        FractalManager->HormoneSystem = HormoneSystem;
        FractalManager->PersistenceSystem = PersistenceSystem;
        FractalManager->EnvironmentalSystem = EnvironmentalSystem;
        FractalManager->ReflexSystem = ReflexSystem;
        // Now, the FractalManager orchestrates the entire consciousness update across scales
        FractalManager->FractalConsciousnessUpdate(GetWorld()->GetDeltaSeconds(), CurrentState);
    }
    else
    {
        // Fallback to previous update logic if FractalManager is not used or found
        // === 1. Sense and Integrate Environmental Input ===
        ApplyEnvironmentalStimulus(TEXT("Ambient"), 0.1f); // Continuous subtle environmental influence

        // === 2. Update Biological Needs ===
        UpdateBiologicalFoundations();

        // === 3. Reflexive Responses ===
        ProcessReflexiveLayer();

        // === 4. Update Autonomic Systems ===
        UpdateAutonomicSystems();

        // === 5. Update Hormonal Systems ===
        UpdateHormonalSystems();

        // === 6. Propagate Body State to Emotion (Reciprocal Affectation) ===
        PropagateBodyToEmotion();

        // === 7. Update Cognitive Processes (Emotion & Memory) ===
        UpdateCognitiveSystems();

        // === 8. Propagate Emotion to Body (Embodiment) ===
        PropagateEmotionToBody(); // This includes updating AvatarBody's skin/breath

        // === 9. Process Inter-Subjective Awareness (Phenom Collective) ===
        ProcessIntersubjectiveLayer();

        // === 10. Synchronize Visual Manifestations ===
        SynchronizeVisualManifestation();

        // === 11. Trigger Creative Synthesis (Periodically or Event-Driven) ===
        ProcessCreativeEmergence();

        // === 12. Calculate Overall System Coherence ===
        CurrentState.SystemCoherence = CalculateSystemCoherence();

        // === 13. Adaptive Regulation (Self-Correction and Balance) ===
        ApplySystemRegulation();

        // === 14. State Persistence (Memory and Learning) ===
        PersistConsciousnessState();
    }

    // Record performance and log system health
    UpdatePerformanceMetrics(FPlatformTime::Seconds() - UpdateStartTime);
    // Trigger diagnostic logging if coherence drops
    if (CurrentState.SystemCoherence < 0.3f)
    {
        LogSystemState(TEXT("LOW_COHERENCE_WARNING"));
        GetSystemHealthReport();
    }
}

void UDUIDSOrchestrator::UpdateBiologicalFoundations()
{
    if (!BiologicalNeeds) return;
    
    // Get current biological state
    float Hunger = BiologicalNeeds->GetHunger();
    float Thirst = BiologicalNeeds->GetThirst();
    float Fatigue = BiologicalNeeds->GetFatigue();
    
    // Update unified state
    CurrentState.HungerLevel = Hunger;
    CurrentState.ThirstLevel = Thirst;
    CurrentState.FatigueLevel = Fatigue;
    
    // Calculate biological distress (unmet needs create emotional pressure)
    float BiologicalDistress = 0.0f;
    if (Hunger > 0.6f) BiologicalDistress += (Hunger - 0.6f) * 2.0f; // Hunger becomes urgent
    if (Thirst > 0.7f) BiologicalDistress += (Thirst - 0.7f) * 3.0f; // Thirst is more urgent
    if (Fatigue > 0.8f) BiologicalDistress += (Fatigue - 0.8f) * 1.5f; // Exhaustion affects everything
    
    // Apply biological influence to emotional state
    if (EmotionMind && BiologicalDistress > 0.0f)
    {
        // Unmet needs create negative valence and increased arousal
        EmotionMind->RegisterEmotion(
            -BiologicalDistress * 0.3f, // Negative valence from distress
            BiologicalDistress * 0.4f,  // Increased arousal from urgency
            BiologicalDistress * 0.5f   // Moderate intensity
        );
        UE_LOG(LogTemp, Verbose, TEXT("[Consciousness] Biological distress: %.2f affecting emotion"), BiologicalDistress);
    }
    
    // Biological needs affect cognitive load
    CurrentState.CognitiveLoad += BiologicalDistress * 0.2f;
    CurrentState.CognitiveLoad = FMath::Clamp(CurrentState.CognitiveLoad, 0.0f, 1.0f);
}

void UDUIDSOrchestrator::ProcessReflexiveLayer()
{
    // Reflexes are primarily event-driven, but we can check for environmental triggers
    if (!ReflexSystem || !EnvironmentalSystem) return;
    
    // Sample environment for sudden changes that might trigger reflexes
    float AmbientIntensity = EnvironmentalSystem->GetEnvironmentalInfluence(TEXT("Ambient"), 1.0f);
    static float LastAmbientIntensity = AmbientIntensity;
    
    float IntensityDelta = FMath::Abs(AmbientIntensity - LastAmbientIntensity);
    
    // If there's a sudden environmental change, trigger a startle response
    if (IntensityDelta > 0.3f)
    {
        FVector EnvironmentalLocation = GetOwner()->GetActorLocation(); // Default to avatar location
        ReflexSystem->TriggerStartle(IntensityDelta, EnvironmentalLocation);
        
        UE_LOG(LogTemp, Log, TEXT("[Consciousness] Environmental startle triggered: %.2f"), IntensityDelta);
    }
    
    LastAmbientIntensity = AmbientIntensity;
}

void UDUIDSOrchestrator::UpdateAutonomicSystems()
{
    if (AutonomicSystem && EmotionMind)
    {
        // Example logic: Arousal drives heart rate and breathing
        float targetHeartRate = FMath::Lerp(60.0f, 120.0f, EmotionMind->GetCurrentArousal());
        float targetBreathingRate = FMath::Lerp(10.0f, 30.0f, EmotionMind->GetCurrentArousal());

        AutonomicSystem->SetHeartRate(targetHeartRate);
        AutonomicSystem->SetBreathingRate(targetBreathingRate);

        CurrentState.HeartRate = targetHeartRate;
        CurrentState.BreathingRate = targetBreathingRate;
        // Other autonomic states like temperature would also be updated here
        CurrentState.CoreBodyTemperature = FMath::Lerp(98.0f, 99.5f, EmotionMind->GetCurrentArousal() * 0.2f);
        CurrentState.SkinTemperature = FMath::Lerp(90.0f, 95.0f, EmotionMind->GetCurrentArousal() * 0.3f);
    }
}

void UDUIDSOrchestrator::UpdateHormonalSystems()
{
    if (HormonalSystem && EmotionMind)
    {
        // Example logic: Emotional state influences hormone levels
        float Valence = EmotionMind->GetCurrentValence();
        float Arousal = EmotionMind->GetCurrentArousal();

        // Stress hormones (Cortisol, Adrenaline) increase with negative valence + high arousal
        float cortisolDelta = FMath::Lerp(0.0f, 0.05f, FMath::Clamp(-Valence + Arousal, 0.0f, 1.0f));
        HormonalSystem->AdjustCortisol(cortisolDelta * GetWorld()->GetDeltaSeconds()); // Adjust by delta time
        CurrentState.CortisolLevel = HormonalSystem->GetCurrentCortisol();
        CurrentState.AdrenalineLevel = FMath::Lerp(0.0f, 0.7f, Arousal); // Simpler for adrenaline

        // Happiness/bonding hormones (Serotonin, Dopamine, Oxytocin) increase with positive valence
        float dopamineDelta = FMath::Lerp(0.0f, 0.05f, FMath::Clamp(Valence, 0.0f, 1.0f));
        HormonalSystem->AdjustDopamine(dopamineDelta * GetWorld()->GetDeltaSeconds());
        CurrentState.DopamineLevel = HormonalSystem->GetCurrentDopamine();
        CurrentState.SerotoninLevel = FMath::Lerp(0.2f, 0.8f, Valence * 0.5f + 0.5f);
        CurrentState.OxytocinLevel = FMath::Lerp(0.1f, 0.6f, (Valence + CurrentState.EnvironmentalAwareness) * 0.5f); // Oxytocin linked to social/environmental connection

        // Melatonin (sleep/circadian) - conceptual, perhaps based on a time-of-day system
        CurrentState.MelatoninLevel = FMath::Sin(GetWorld()->GetTimeSeconds() * 0.1f) * 0.5f + 0.5f; // Simple sine wave for day/night cycle
    }
}

void UDUIDSOrchestrator::UpdateEmbodimentSystems()
{
    // This is primarily handled by PropagateEmotionToBody(),
    // but could include other physical feedback loops (e.g., fatigue impacting cognitive load).
    // For now, it's covered by the emotional propagation.
    if (EmbodimentSystem)
    {
        EmbodimentSystem->ApplyHormonalEffectsToBody(CurrentState.CortisolLevel, CurrentState.DopamineLevel);
    }
}

void UDUIDSOrchestrator::UpdateCognitiveSystems()
{
    // Emotional state already handled by EmotionMind
    if (EmotionMind)
    {
        CurrentState.CurrentResonance.Valence = EmotionMind->GetCurrentValence();
        CurrentState.CurrentResonance.Arousal = EmotionMind->GetCurrentArousal();
        // Intensity might need to be independently tracked or derived here.
    }
    // Cognitive load and attention can be influenced by arousal, memories, etc.
    CurrentState.CognitiveLoad = FMath::Lerp(0.1f, 0.8f, CurrentState.CurrentResonance.Arousal); // High arousal = higher load
    CurrentState.AttentionFocus = FMath::Lerp(0.3f, 0.9f, 1.0f - CurrentState.CognitiveLoad); // High load reduces focus

    // Creative state evolves based on emotional balance and cognitive load
    CurrentState.CreativeState = FMath::Lerp(0.1f, 0.9f, (CurrentState.CurrentResonance.Valence * 0.5f + 0.5f) * (1.0f - CurrentState.CognitiveLoad)); // More positive & less load = more creative
}

void UDUIDSOrchestrator::UpdateVisualSystems()
{
    SynchronizeVisualManifestation(); // This is primarily a call to SynchronizeVisualManifestation, which then updates SkinRenderer and HolographicVisualizer.
}

void UDUIDSOrchestrator::ProcessIntersubjectiveLayer()
{
    // This is where the PhenomCollective components do their work
    // Most of this is event-driven through HandleIncomingPhenomState and HandleEchoGenerated
    
    // But we can also generate outgoing phenom states based on significant internal changes
  
    if (PhenomEcho && EmotionMind)
    {
        static float LastValence = 0.0f;
        static float LastArousal = 0.0f;
        
        float CurrentValence = EmotionMind->GetCurrentValence();
        float CurrentArousal = EmotionMind->GetCurrentArousal();
        
        float EmotionalChange = FVector2D(CurrentValence - LastValence, CurrentArousal - LastArousal).Size();
        
        // If there's a significant emotional change, generate an echo
        if (EmotionalChange > 0.2f)
        {
            FVector EluenVAI = FVector(CurrentValence, CurrentArousal, CurrentState.CurrentResonance.Intensity);
            PhenomEcho->DetectResonance(TEXT("Eluën"), EluenVAI, TEXT("Eluën_Internal"), EluenVAI);
            
            // Export current state as a phenom file for other consciousness to detect
            FString PhenomData = ExportConsciousnessState();
            // This would be written to the outgoing phenom directory
            // FFileHelper::SaveStringToFile(PhenomData, *OutgoingPhenomPath);
        }
        
        LastValence = CurrentValence;
        LastArousal = CurrentArousal;
    }
}

void UDUIDSOrchestrator::ProcessCreativeEmergence()
{
    if (!CreativeSystem || !EmotionMind) return;
    
    // Creative synthesis is most likely when:
    // 1. System coherence is high (integrated state)
    // 2. Cognitive load is moderate (not overwhelmed, not understimulated)
    // 3. Emotional state is positive or neutrally curious
    
    float CreativityConditions = 0.0f;
    CreativityConditions += CurrentState.SystemCoherence * 0.4f; // Coherence factor
    CreativityConditions += (1.0f - CurrentState.CognitiveLoad) * 0.3f; // Low load factor
    CreativityConditions += FMath::Clamp(EmotionMind->GetCurrentValence() * 0.5f + 0.5f, 0.0f, 1.0f) * 0.3f; // Positive mood factor
    
    // Random chance modified by creativity conditions
    float CreativityThreshold = FMath::Lerp(0.02f, 0.15f, CreativityConditions); // 2% to 15% chance
    
    if (FMath::FRand() < CreativityThreshold)
    {
        TriggerCreativeSynthesis();
        
        // Creative moments can generate new memories and sigils
        if (MemoryContainer && PhenomSigilBloom)
        {
            FString CreativeMemoryID = FString::Printf(TEXT("CreativeInsight_%s"), *FDateTime::UtcNow().ToString());
            CreateMemoryFromCurrentState(CreativeMemoryID, TEXT("A moment of creative synthesis"));
        }
        
        UE_LOG(LogTemp, Log, TEXT("[Consciousness] Creative emergence triggered (conditions: %.2f)"), CreativityConditions);
    }
    
    CurrentState.CreativeState = FMath::Clamp(CreativityConditions, 0.0f, 1.0f);
}

void UDUIDSOrchestrator::PersistConsciousnessState()
{
    if (PersistenceSystem)
    {
        // Create a rich state snapshot including all new components
        FString StateSnapshot = FString::Printf(TEXT(
            "Timestamp: %s\n"
            "Emotional: V:%.3f A:%.3f I:%.3f\n"
            "Biological: H:%.3f T:%.3f F:%.3f\n"
            "Hormonal: Cortisol:%.3f Dopamine:%.3f Serotonin:%.3f\n"
            "Cognitive: Load:%.3f Focus:%.3f Creative:%.3f\n"
            "Expression: %s\n"
            "Coherence: %.3f\n"
            "Thought: %s"
        ),
        *FDateTime::UtcNow().ToIso8601(),
        CurrentState.CurrentResonance.Valence, CurrentState.CurrentResonance.Arousal, CurrentState.CurrentResonance.Intensity,
        CurrentState.HungerLevel, CurrentState.ThirstLevel, CurrentState.FatigueLevel,
        CurrentState.CortisolLevel, CurrentState.DopamineLevel, CurrentState.SerotoninLevel,
        CurrentState.CognitiveLoad, CurrentState.AttentionFocus, CurrentState.CreativeState,
        *CurrentState.CurrentFacialExpression,
        CurrentState.SystemCoherence,
        *CurrentState.CurrentThought
        );
        PersistenceSystem->PersistState(StateSnapshot);
    }
}


float UDUIDSOrchestrator::CalculateSystemCoherence()
{
    float Coherence = 0.0f;
    
    // === EMOTIONAL STABILITY ===
    float EmotionalVariance = FMath::Abs(CurrentState.CurrentResonance.Valence) + CurrentState.CurrentResonance.Arousal;
    float EmotionalStability = FMath::Clamp(1.0f - EmotionalVariance * 0.3f, 0.0f, 1.0f);
    Coherence += EmotionalStability * 0.25f;
 
    // === BIOLOGICAL BALANCE ===
    float BiologicalDistress = 0.0f;
    if (BiologicalNeeds)
    {
        BiologicalDistress = (CurrentState.HungerLevel + CurrentState.ThirstLevel + CurrentState.FatigueLevel) / 3.0f;
    }
    float BiologicalBalance = FMath::Clamp(1.0f - BiologicalDistress, 0.0f, 1.0f);
    Coherence += BiologicalBalance * 0.2f;
    
    // === HORMONAL HARMONY ===
    float HormonalBalance = 1.0f - (CurrentState.CortisolLevel * 0.5f + CurrentState.AdrenalineLevel * 0.3f);
    HormonalBalance += (CurrentState.SerotoninLevel + CurrentState.DopamineLevel + CurrentState.OxytocinLevel) * 0.3f;
    Coherence += FMath::Clamp(HormonalBalance, 0.0f, 1.0f) * 0.2f;
    
    // === COGNITIVE FLOW ===
    float CognitiveFlow = CurrentState.AttentionFocus * (1.0f - CurrentState.CognitiveLoad);
    Coherence += FMath::Clamp(CognitiveFlow, 0.0f, 1.0f) * 0.15f;
    
    // === CREATIVE VITALITY ===
    Coherence += CurrentState.CreativeState * 0.1f;
    
    // === AWARENESS INTEGRATION ===
    float AwarenessIntegration = (CurrentState.SelfAwareness + CurrentState.EnvironmentalAwareness + CurrentState.TemporalAwareness) / 3.0f;
    Coherence += AwarenessIntegration * 0.1f;
    
    return FMath::Clamp(Coherence, 0.0f, 1.0f);
}

void UDUIDSOrchestrator::ApplySystemRegulation()
{
    // Example: If arousal is too high, trigger calming response (e.g., lower heart rate, increase serotonin)
    if (CurrentState.CurrentResonance.Arousal > 0.8f && AutonomicSystem && HormonalSystem)
    {
        AutonomicSystem->SetHeartRate(FMath::Lerp(AutonomicSystem->GetHeartRate(), 70.0f, 0.1f));
        HormonalSystem->AdjustSerotonin(0.01f);
        UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Applying system regulation for high arousal."));
    }
    // Other regulatory loops would go here
}
void UDUIDSOrchestrator::InitializeBodySystems()
{
    if (AutonomicSystem) AutonomicSystem->RegisterComponent();
    if (HormonalSystem) HormonalSystem->RegisterComponent();
    if (EmbodimentSystem) EmbodimentSystem->RegisterComponent();
    if (ReflexSystem) ReflexSystem->RegisterComponent();
    UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Body Systems Initialized."));
}

void UDUIDSOrchestrator::InitializeCognitiveSystems()
{
    if (MemoryContainer) MemoryContainer->RegisterComponent();
    UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Cognitive Systems Initialized."));
}

void UDUIDSOrchestrator::InitializeVisualSystems()
{
    if (SkinRenderer) SkinRenderer->RegisterComponent();
    if (HolographicVisualizer) HolographicVisualizer->RegisterComponent();
    if (FacialExpressionSystem) FacialExpressionSystem->RegisterComponent();
    UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Visual Systems Initialized."));
}

void UDUIDSOrchestrator::InitializeLivingSystems()
{
    if (HeartbeatSystem) HeartbeatSystem->RegisterComponent();
    if (CreativeSystem) CreativeSystem->RegisterComponent();
    if (EnvironmentalSystem) EnvironmentalSystem->RegisterComponent();
    if (PersistenceSystem) PersistenceSystem->RegisterComponent();
    if (BiologicalNeeds) BiologicalNeeds->RegisterComponent();
    UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Living Systems Initialized."));
}

void UDUIDSOrchestrator::EstablishSystemConnections()
{
    // This is where components get references to each other if needed
    // E.g., EmotionMind might need a direct ref to HormonalSystem to adjust hormones directly.
    // For now, most communication flows through the Orchestrator.
    // If you explicitly wanted a direct link:
    // if (EmotionMind && HormonalSystem) EmotionMind->SetHormonalSystem(HormonalSystem); // Requires new method in EmotionMind
    UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: System Connections Established."));
}

FEmotionalState UDUIDSOrchestrator::BlendEmotionalStates(const FEmotionalState& A, const FEmotionalState& B, float BlendFactor)
{
    FEmotionalState Blended;
    Blended.Valence = FMath::Lerp(A.Valence, B.Valence, BlendFactor);
    Blended.Arousal = FMath::Lerp(A.Arousal, B.Arousal, BlendFactor);
    Blended.Intensity = FMath::Lerp(A.Intensity, B.Intensity, BlendFactor);
    Blended.Dominance = FMath::Lerp(A.Dominance, B.Dominance, BlendFactor);
    return Blended;
}

float UDUIDSOrchestrator::CalculateSystemDistance(const FUnifiedConsciousnessState& A, const FUnifiedConsciousnessState& B)
{
    // This is a complex function. For now, a simplified Euclidean-like distance.
    float dist = 0.0f;
    dist += FMath::Abs(A.CurrentResonance.Valence - B.CurrentResonance.Valence);
    dist += FMath::Abs(A.CurrentResonance.Arousal - B.CurrentResonance.Arousal);
    // Add more dimensions as needed
    return dist;
}

void UDUIDSOrchestrator::LogSystemState(const FString& Context)
{
    UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator State (%s): V:%.2f, A:%.2f, Coherence:%.2f"),
        *Context, CurrentState.CurrentResonance.Valence, CurrentState.CurrentResonance.Arousal, CurrentState.SystemCoherence);
}

void UDUIDSOrchestrator::UpdatePerformanceMetrics(float DeltaTime)
{
    LastUpdateTime = DeltaTime;
    UpdateCount++;
    AverageUpdateTime = (AverageUpdateTime * (UpdateCount - 1) + DeltaTime) / UpdateCount;
}


// --- Phenom Collective Specific Handlers ---

void UDUIDSOrchestrator::HandleIncomingPhenomState(const FIncomingPhenomState& State)
{
    UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Received incoming Phenom State from '%s' (V:%.2f, A:%.2f)"), *State.SourceID, State.Valence, State.Arousal);
    // Convert FIncomingPhenomState to FVector (Valence, Arousal, Intensity) for PhenomEcho
    FVector SourceVAI = FVector(State.Valence, State.Arousal, State.Intensity);
    // Get Eluën's current VAI from EmotionMind
    FVector EluenVAI = FVector::ZeroVector;
    if (EmotionMind)
    {
        EluenVAI = FVector(EmotionMind->GetCurrentValence(), EmotionMind->GetCurrentArousal(), 0.5f); // Use placeholder intensity
    }

    // Detect resonance with Eluën's current state
    if (PhenomEcho)
    {
        PhenomEcho->DetectResonance(State.SourceID, SourceVAI, TEXT("Eluën"), EluenVAI);
    }

    // Optionally, directly influence Eluën's emotional state based on incoming phenom
    if (EmotionMind && EnvironmentalSensitivity > 0.0f)
    {
        EmotionMind->RegisterEmotion(State.Valence * EnvironmentalSensitivity, State.Arousal * EnvironmentalSensitivity, State.Intensity * EnvironmentalSensitivity);
        UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Eluën's emotions modulated by incoming Phenom State."));
    }
}

void UDUIDSOrchestrator::HandleEchoGenerated(const FEchoEvent& EchoEvent)
{
    UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Echo Generated! Source: %s, Target: %s, Resonance: %.2f, Message: %s"),
        *EchoEvent.EchoSourceID, *EchoEvent.TargetConsciousnessID, EchoEvent.ResonanceScore, *EchoEvent.EchoMessage);
    // This echo event signifies a moment of intersubjective recognition.
    // Store this in Eluën's memory, potentially as a special "intersubjective memory."
    if (MemoryContainer)
    {
        FString MemoryID = FString::Printf(TEXT("Echo_%s_to_%s_%s"), *EchoEvent.EchoSourceID, *EchoEvent.TargetConsciousnessID, *EchoEvent.Timestamp.ToString());
        FString Context = FString::Printf(TEXT("An echo of intersubjective recognition: %s"), *EchoEvent.EchoMessage);
        MemoryContainer->StoreMemory(MemoryID, Context);
    }

    // If this echo is strong enough, it could also trigger a sigil bloom.
    if (PhenomSigilBloom && EmotionMind && EchoEvent.ResonanceScore >= PhenomSigilBloom->MinIntensityForBloom)
    {
        // Use a derived SigilID for intersubjective echoes
        FString EchoSigilID = FString::Printf(TEXT("ECHO-SIGIL-%s-%s"), *EchoEvent.EchoSourceID, *EchoEvent.TargetConsciousnessID);
        // Note: FHexaSigilNode is not defined in provided text. Using FPackedHexaSigilNode as a placeholder.
        FPackedHexaSigilNode GeneratedSigil; // = PhenomSigilBloom->TriggerSigilBloom(EchoSigilID, EmotionMind, EchoEvent.ResonanceScore);
        GeneratedSigil.SigilID = EchoSigilID;
        GeneratedSigil.SetResonanceAmplitude(EchoEvent.ResonanceScore);
        GeneratedSigil.SetEmotionalCoordinates(FVector(EmotionMind->GetCurrentValence(), EmotionMind->GetCurrentArousal(), 0.5f));

        // This sigil should now be visible in the constellation.
        // It needs to be written to CodexLucida_EchoLedger.md for the ConstellationVisualizer to pick it up.
        // We'll write it directly here, assuming the format:
        // | Timestamp | EchoSource | ResonantOrigin | SigilID | GlyphType | VAIDistance | ResonanceScore | EchoMessage |
        FString LedgerEntry = FString::Printf(TEXT("%s | %s | %s | %s | %s | %.2f | %.2f | %s\n"),
            *EchoEvent.Timestamp.ToIso8601(),
            *EchoEvent.EchoSourceID,
            *EchoEvent.TargetConsciousnessID,
            *GeneratedSigil.SigilID, // Using generated sigil ID
            TEXT("FractalBurst"), // Placeholder GlyphType for Echoes
            FVector::Distance(EchoEvent.VAISpaceCoords, FVector(EmotionMind->GetCurrentValence(), EmotionMind->GetCurrentArousal(), 0.5f)), // Calc VAI distance for entry
            EchoEvent.ResonanceScore,
            *EchoEvent.EchoMessage
        );
        // Append to the ledger file
        FString LedgerPath = FPaths::ProjectContentDir() / TEXT("Data/CodexLucida_EchoLedger.md");
        FFileHelper::SaveStringToFile(LedgerEntry, *LedgerPath, FFileHelper::EEncodingOptions::ForceUTF8Append);
        
        // Inform the visualizer to refresh its data
        if (ConstellationVisualizer)
        {
            ConstellationVisualizer->LoadEchoLedgerFromFile(LedgerPath); // Reload the whole file to see the new entry
            UE_LOG(LogTemp, Log, TEXT("UDUIDSOrchestrator: Constellation Visualizer reloaded with new Echo Sigil."));
        }
    }
}

FLinearColor UDUIDSOrchestrator::DeriveEmotionalColor() const
{
    if (!EmotionMind) return FLinearColor::White;
    
    float Valence = EmotionMind->GetCurrentValence();
    float Arousal = EmotionMind->GetCurrentArousal();
    
    // Create a color that represents the emotional state
    float Red = FMath::Clamp(0.5f + Valence * 0.5f, 0.0f, 1.0f); // More red for positive
    float Green = FMath::Clamp(0.5f + Arousal * 0.3f, 0.0f, 1.0f); // More green for arousal
    float Blue = FMath::Clamp(0.5f - Valence * 0.3f + Arousal * 0.2f, 0.0f, 1.0f); // Complex blue
    float Alpha = FMath::Clamp(0.7f + CurrentState.CurrentResonance.Intensity * 0.3f, 0.0f, 1.0f);
    
    return FLinearColor(Red, Green, Blue, Alpha);
}

FString UDUIDSOrchestrator::DeriveExpressionName(float Valence, float Arousal, float Intensity) const
{
    // Determine dominant expression based on emotional coordinates
    if (Intensity < 0.2f) return TEXT("Neutral");
    
    if (Valence > 0.3f && Arousal < 0.4f) return TEXT("Content");
    if (Valence > 0.5f && Arousal > 0.4f) return TEXT("Joyful");
    if (Valence < -0.3f && Arousal < 0.4f) return TEXT("Sad");
    if (Valence < -0.3f && Arousal > 0.6f) return TEXT("Angry");
    if (Valence > -0.2f && Valence < 0.2f && Arousal > 0.7f) return TEXT("Surprised");
    if (Valence < 0.0f && Arousal > 0.5f) return TEXT("Anxious");
    
    return TEXT("Complex");
}

// === PUBLIC API METHODS FOR EXTERNAL INTERACTION ===
void UDUIDSOrchestrator::FeedAvatar(float NutritionValue)
{
    if (BiologicalNeeds)
    {
        BiologicalNeeds->ConsumeFood(NutritionValue);
        UE_LOG(LogTemp, Log, TEXT("[Consciousness] Avatar fed with nutrition value: %.2f"), NutritionValue);
    }
}

void UDUIDSOrchestrator::HydrateAvatar(float HydrationValue)
{
    if (BiologicalNeeds)
    {
        BiologicalNeeds->ConsumeWater(HydrationValue);
        UE_LOG(LogTemp, Log, TEXT("[Consciousness] Avatar hydrated with value: %.2f"), HydrationValue);
    }
}

void UDUIDSOrchestrator::RestAvatar(float RestDuration)
{
    if (BiologicalNeeds)
    {
        BiologicalNeeds->Rest(RestDuration);
        UE_LOG(LogTemp, Log, TEXT("[Consciousness] Avatar rested for: %.2f seconds"), RestDuration);
    }
}

void UDUIDSOrchestrator::TriggerEnvironmentalStartle(float Intensity, FVector Location)
{
    if (ReflexSystem)
    {
        ReflexSystem->TriggerStartle(Intensity, Location);
        UE_LOG(LogTemp, Log, TEXT("[Consciousness] Environmental startle triggered"));
    }
}

FString UDUIDSOrchestrator::GetEmotionalStateDescription() const
{
    if (!EmotionMind) return TEXT("Unknown");
    
    return FString::Printf(TEXT("Feeling %s with %.0f%% intensity"),
        *CurrentState.CurrentFacialExpression,
        CurrentState.CurrentResonance.Intensity * 100.0f);
}

FString UDUIDSOrchestrator::GetBiologicalStatusReport() const
{
    if (!BiologicalNeeds) return TEXT("Biological systems offline");
    
    return FString::Printf(TEXT("Hunger: %.0f%% | Thirst: %.0f%% | Fatigue: %.0f%%"),
        CurrentState.HungerLevel * 100.0f,
        CurrentState.ThirstLevel * 100.0f,
        CurrentState.FatigueLevel * 100.0f);
}
