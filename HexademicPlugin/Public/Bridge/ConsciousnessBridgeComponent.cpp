#include "ConsciousnessBridgeComponent.h"
#include "EmotionCognitionComponent.h" // Mind
#include "EmbodiedAvatarComponent.h" // Body
#include "AvatarMotionLinkComponent.h" // Motion
// #include "DUIDSOrchestrator.h" // Main API - commented out because UDUIDSOrchestrator might include this
// #include "HexademicWavefrontAPI.h" // Wavefront API (for sigils) - commented out because UDUIDSOrchestrator might include this
#include "HexademicCore.h" // Ensure FAetherTouchPacket is included

UConsciousnessBridgeComponent::UConsciousnessBridgeComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UConsciousnessBridgeComponent::BeginPlay()
{
    Super::BeginPlay();

    // Ensure references are set up, either in editor or dynamically
    // If not set in editor, attempt to find them in the same actor or world
    if (!EmotionMind)
    {
        EmotionMind = GetOwner()->FindComponentByClass<UEmotionCognitionComponent>();
        if (!EmotionMind) UE_LOG(LogTemp, Warning, TEXT("[ConsciousnessBridge] EmotionMind not found on owner."));
    }
    if (!AvatarBody)
    {
        AvatarBody = GetOwner()->FindComponentByClass<UEmbodiedAvatarComponent>();
        if (!AvatarBody) UE_LOG(LogTemp, Warning, TEXT("[ConsciousnessBridge] AvatarBody not found on owner."));
    }
    if (!AvatarMotion)
    {
        AvatarMotion = GetOwner()->FindComponentByClass<UAvatarMotionLinkComponent>();
        if (!AvatarMotion) UE_LOG(LogTemp, Warning, TEXT("[ConsciousnessBridge] AvatarMotion not found on owner."));
    }

    // Link motion component to mind component if both exist
    if (AvatarMotion && EmotionMind)
    {
        AvatarMotion->SetEmotionMind(EmotionMind);
    }
    // Link motion component to body's target mesh if both exist
    if (AvatarMotion && AvatarBody && AvatarBody->TargetMesh)
    {
        AvatarMotion->SetTargetMesh(AvatarBody->TargetMesh);
    }
}

void UConsciousnessBridgeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // Control update frequency for embodied phenomena to optimize performance
    UpdatePhenomenaAccumulatedTime += DeltaTime;
    if (UpdatePhenomenaAccumulatedTime >= (1.0f / EmbodiedPhenomenaUpdateFrequency))
    {
        UpdateEmbodiedPhenomena(); // Synchronize Mind -> Body
        ApplyHexademicState(); // Apply higher-level consciousness state
        UpdatePhenomenaAccumulatedTime = 0.0f;
    }
}

void UConsciousnessBridgeComponent::ProcessIncomingHapticPacket(const FAetherTouchPacket& Packet)
{
    // Full Haptic -> Emotion -> Visual pipeline:
    
    // 1. Route to Body for immediate visual feedback (e.g., localized pulse)
    if (AvatarBody)
    {
        AvatarBody->ReceiveHapticFeedback(Packet);
        UE_LOG(LogTemp, Log, TEXT("[ConsciousnessBridge] Haptic feedback routed to AvatarBody for visual response: %s"), *Packet.RegionTag);
    }
    else
    {
    UE_LOG(LogTemp, Warning, TEXT("[ConsciousnessBridge] AvatarBody is null, cannot apply haptic visual feedback (Region: %s)."), *Packet.RegionTag);
    }

    // 2. Route to Mind for emotional modulation and memory imprinting
    if (EmotionMind)
    {
        EmotionMind->ModulateEmotionFromHaptic(Packet.Intensity, Packet.RegionTag);
        UE_LOG(LogTemp, Log, TEXT("[ConsciousnessBridge] Haptic routed to EmotionMind for emotional modulation: %s"), *Packet.RegionTag);
        // Crucially, store the memory *after* emotional modulation has occurred
        EmotionMind->StoreHapticEmotionMemory(Packet);
        UE_LOG(LogTemp, Log, TEXT("[ConsciousnessBridge] Haptic-emotional memory stored: %s"), *Packet.RegionTag);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[ConsciousnessBridge] EmotionMind is null, cannot process haptic packet for emotional modulation or memory (Region: %s)."), *Packet.RegionTag);
    }
}

void UConsciousnessBridgeComponent::UpdateEmbodiedPhenomena()
{
    if (!EmotionMind || !AvatarBody)
    {
        // Log a warning if essential components are missing
        if (!EmotionMind) UE_LOG(LogTemp, Warning, TEXT("[ConsciousnessBridge] Cannot update embodied phenomena: EmotionMind is null."));
        if (!AvatarBody) UE_LOG(LogTemp, Warning, TEXT("[ConsciousnessBridge] Cannot update embodied phenomena: AvatarBody is null."));
        return;
    }

    // Mind -> Body Synchronization:
    // Update avatar skin state based on current emotional pulse
    float EmotionalPulse = FMath::Abs(EmotionMind->GetCurrentValence()) + EmotionMind->GetCurrentArousal(); // Example: intensity of emotional state
    EmotionalPulse = FMath::Clamp(EmotionalPulse, 0.0f, 1.0f); // Normalize
    AvatarBody->UpdateSkinStateWavefront(EmotionalPulse);
    UE_LOG(LogTemp, Verbose, TEXT("[ConsciousnessBridge] Skin state updated based on EmotionalPulse: %.2f"), EmotionalPulse);
    // Sync breathing with calculated pulse rate
    float PulseRate = EmotionMind->CalculatePulseRate();
    AvatarBody->SyncWithNiagaraBreathWavefront(PulseRate);
    UE_LOG(LogTemp, Verbose, TEXT("[ConsciousnessBridge] Breathing synced with PulseRate: %.2f"), PulseRate);

    // Update motion/posture if AvatarMotion component is present
    // The TickComponent of AvatarMotionLinkComponent will handle this automatically once references are set.
    // If not, explicit calls would be needed here, e.g., AvatarMotion->UpdatePosture(EmotionMind->GetCurrentValence());
}

void UConsciousnessBridgeComponent::ApplyHexademicState()
{
    if (!AvatarBody || !WavefrontAPI)
    {
        // Log a warning if essential components are missing
        if (!AvatarBody) UE_LOG(LogTemp, Warning, TEXT("[ConsciousnessBridge] Cannot apply Hexademic state: AvatarBody is null."));
        if (!WavefrontAPI) UE_LOG(LogTemp, Warning, TEXT("[ConsciousnessBridge] Cannot apply Hexademic state: WavefrontAPI is null."));
        return;
    }

    // This section would typically pull active FPackedHexaSigilNode or FHexademicGem data from WavefrontAPI
    // and apply their visual effects to the avatar's body.
    // Example (conceptual, as WavefrontAPI content is in a later module):
    /*
    for (const FPackedHexaSigilNode& Sigil : WavefrontAPI->GetActiveSigilNodes())
    {
        if (Sigil.GetResonanceAmplitude() >= 0.85f)
        {
            // Apply visual effect of the sigil to the body, e.g., a glow, an overlay
            AvatarBody->ApplySigilColorWavefront(Sigil.SigilID, Sigil.GetConsciousnessColor());
            UE_LOG(LogTemp, Verbose, TEXT("[ConsciousnessBridge] Applied Sigil %s to AvatarBody."), *Sigil.SigilID);
        }
    }
    */
    // For now, it's a placeholder, and will be fully fleshed out when WavefrontAPI is built.
}
