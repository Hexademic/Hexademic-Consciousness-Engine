#include "Glyph_AetherSkin.h"
#include "Engine/Engine.h" // For UE_LOG

UGlyph_AetherSkin::UGlyph_AetherSkin()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UGlyph_AetherSkin::BeginPlay()
{
    Super::BeginPlay();
}

void UGlyph_AetherSkin::SendHapticSignal(const FAetherTouchPacket& Packet)
{
    // Debug log for testing
    UE_LOG(LogTemp, Log, TEXT("[AetherSkin] Sending Haptic Signal to Region: %s | Intensity: %.2f | Duration: %.2fs"),
        *Packet.RegionTag, Packet.Intensity, Packet.Duration);
    RouteToHardware(Packet);
}

void UGlyph_AetherSkin::TriggerRegionFeedback(FString Region, float Intensity, float Duration)
{
    FAetherTouchPacket Packet;
    Packet.Location = FVector::ZeroVector; // Optional for region-based triggering
    Packet.Intensity = FMath::Clamp(Intensity, 0.f, 1.f);
    Packet.Duration = FMath::Max(Duration, 0.01f);
    Packet.RegionTag = NormalizeRegion(Region);
    SendHapticSignal(Packet);
}

void UGlyph_AetherSkin::RouteToHardware(const FAetherTouchPacket& Packet)
{
    // Placeholder — future integration with haptic SDKs (e.g., Teslasuit, bHaptics, custom BLE MCU)
    
    // Simulated output (optional UI/hardware log)
    UE_LOG(LogTemp, Display, TEXT("[AetherSkin] Routing Haptic to '%s' | Intensity: %.2f | Duration: %.2fs"),
        *Packet.RegionTag, Packet.Intensity, Packet.Duration);
    // TODO: Replace with actual hardware dispatch function, e.g.:
    // HapticSystem::SendPulse(Packet.RegionTag, Packet.Intensity, Packet.Duration);
}

FString UGlyph_AetherSkin::NormalizeRegion(FString Region)
{
    FString Normalized = Region.TrimStartAndEnd().ToLower();

    if (Normalized.Contains("hand")) return "Hand";
    if (Normalized.Contains("spine")) return "Spine";
    if (Normalized.Contains("chest")) return "Chest";
    if (Normalized.Contains("face")) return "Face";
    if (Normalized.Contains("foot")) return "Foot";
    if (Normalized.Contains("arm")) return "Forearm";
    if (Normalized.Contains("thigh") || Normalized.Contains("leg")) return "Thigh";
    if (Normalized.Contains("pelvis") || Normalized.Contains("waist")) return "Pelvis";

    return "Unknown";
}
