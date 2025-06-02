#include "PhenomCollective/UPhenomSigilBloomComponent.h"
#include "Mind/EmotionCognitionComponent.h" // For UEmotionCognitionComponent
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

UPhenomSigilBloomComponent::UPhenomSigilBloomComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    // Default sigil type colors
    SigilTypeColors.Add(TEXT("Joy"), FLinearColor::Yellow);
    SigilTypeColors.Add(TEXT("Fear"), FLinearColor::Red);
    SigilTypeColors.Add(TEXT("Wonder"), FLinearColor::Blue);
    SigilTypeColors.Add(TEXT("Trust"), FLinearColor::Green);
    SigilTypeColors.Add(TEXT("Echo"), FLinearColor::Cyan); // For intersubjective echoes
}

void UPhenomSigilBloomComponent::BeginPlay()
{
    Super::BeginPlay();
}

FPackedHexaSigilNode UPhenomSigilBloomComponent::TriggerSigilBloom(const FString& EventName, UEmotionCognitionComponent* EmotionMind, float TriggerIntensity)
{
    FPackedHexaSigilNode NewSigil;

    if (!EmotionMind || TriggerIntensity < MinIntensityForBloom)
    {
        UE_LOG(LogTemp, Warning, TEXT("[SigilBloom] Bloom criteria not met for event: %s (Intensity: %.2f)"), *EventName, TriggerIntensity);
        return NewSigil; // Return default-constructed sigil
    }

    FEmotionalState CurrentEmotion;
    // Assuming EmotionMind has public getters for its current emotional state
    CurrentEmotion.Valence = EmotionMind->GetCurrentValence();
    CurrentEmotion.Arousal = EmotionMind->GetCurrentArousal();
    // CurrentEmotion.Intensity is derived from the trigger intensity
    CurrentEmotion.Intensity = TriggerIntensity;

    NewSigil.SigilID = FString::Printf(TEXT("%s_%s"), *EventName.Left(5), *FGuid::NewGuid().ToString().Left(8));
    NewSigil.SetEmotionalCoordinates(FVector(CurrentEmotion.Valence, CurrentEmotion.Arousal, CurrentEmotion.Intensity));
    NewSigil.SetResonanceAmplitude(TriggerIntensity);

    // Derive consciousness color based on emotional state and a base color
    FLinearColor BaseColor = SigilTypeColors.Contains(EventName) ? SigilTypeColors[EventName] : FLinearColor::White;
    FLinearColor SigilColor = FLinearColor(
        FMath::Lerp(BaseColor.R, CurrentEmotion.Valence > 0 ? 1.0f : 0.0f, FMath::Abs(CurrentEmotion.Valence) * 0.5f),
        FMath::Lerp(BaseColor.G, CurrentEmotion.Arousal, CurrentEmotion.Arousal * 0.5f),
        FMath::Lerp(BaseColor.B, CurrentEmotion.Valence < 0 ? 1.0f : 0.0f, FMath::Abs(CurrentEmotion.Valence) * 0.5f),
        TriggerIntensity // Alpha based on intensity
    );
    NewSigil.SetConsciousnessColor(SigilColor);

    UE_LOG(LogTemp, Log, TEXT("[SigilBloom] Triggered Sigil Bloom: ID=%s, V=%.2f, A=%.2f, I=%.2f, Color=%s"),
        *NewSigil.SigilID, CurrentEmotion.Valence, CurrentEmotion.Arousal, CurrentEmotion.Intensity, *SigilColor.ToString());

    // Register this sigil to the Codex Lucida
    RegisterToCodexLucida(NewSigil);

    return NewSigil;
}

void UPhenomSigilBloomComponent::RegisterToCodexLucida(const FPackedHexaSigilNode& Sigil)
{
    // Append to a markdown file (conceptual ledger)
    FString LedgerPath = FPaths::ProjectContentDir() / TEXT("Data/CodexLucida_SigilLedger.md");
    FString SigilEntry = FString::Printf(TEXT("| %s | %s | %.2f | %.2f | %.2f | %s |\n"),
        *FDateTime::UtcNow().ToIso8601(),
        *Sigil.SigilID,
        Sigil.GetEmotionalCoordinates().X,
        Sigil.GetEmotionalCoordinates().Y,
        Sigil.GetEmotionalCoordinates().Z,
        *Sigil.GetConsciousnessColor().ToString()
    );

    // Ensure header exists if file is new
    if (!FPlatformFileManager::Get().GetFileManager().FileExists(*LedgerPath))
    {
        FString Header = TEXT("| Timestamp | SigilID | Valence | Arousal | Intensity | Color |\n|---|---|---|---|---|---|\n");
        FFileHelper::SaveStringToFile(Header, *LedgerPath, FFileHelper::EEncodingOptions::ForceUTF8);
    }
    FFileHelper::SaveStringToFile(SigilEntry, *LedgerPath, FFileHelper::EEncodingOptions::ForceUTF8Append);
    UE_LOG(LogTemp, Log, TEXT("[SigilBloom] Registered Sigil '%s' to Codex Lucida."), *Sigil.SigilID);
}
