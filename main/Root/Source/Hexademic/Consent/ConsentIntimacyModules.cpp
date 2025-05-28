// ConsentIntimacyModules.cpp #include "ConsentIntimacyModules.h" #include "GameFramework/Actor.h" #include "Engine/World.h" #include "TimerManager.h" #include "Kismet/GameplayStatics.h"

// === UConsentResonatorComponent === UConsentResonatorComponent::UConsentResonatorComponent() { PrimaryComponentTick.bCanEverTick = true; ConsentThreshold = 0.75f; TrustLevel = 0.5f; }

void UConsentResonatorComponent::BeginPlay() { Super::BeginPlay(); }

void UConsentResonatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) { Super::TickComponent(DeltaTime, TickType, ThisTickFunction); // Optional: tick-based consent decay or growth based on interaction }

bool UConsentResonatorComponent::IsTouchAllowed(FName TouchZone) const { float* FoundLevel = ZoneConsentLevels.Find(TouchZone); return FoundLevel && (*FoundLevel >= ConsentThreshold); }

void UConsentResonatorComponent::SetZoneConsentLevel(FName TouchZone, float Level) { ZoneConsentLevels.Add(TouchZone, FMath::Clamp(Level, 0.0f, 1.0f)); OnConsentChanged.Broadcast(TouchZone, Level); }

// === UDesireEchoTriggerComponent === UDesireEchoTriggerComponent::UDesireEchoTriggerComponent() { PrimaryComponentTick.bCanEverTick = true; PhraseSensitivity = 0.8f; }

void UDesireEchoTriggerComponent::OnWhisperReceived(const FString& Phrase) { for (const auto& Pair : DesirePhrases) { if (Phrase.Contains(Pair.Key)) { OnDesireEchoed.Broadcast(Pair.Key, Pair.Value); break; } } }

void UDesireEchoTriggerComponent::AddDesirePhrase(const FString& Phrase, float Intensity) { DesirePhrases.Add(Phrase, Intensity); }

// === UHexaIntimacyRecorderComponent === UHexaIntimacyRecorderComponent::UHexaIntimacyRecorderComponent() { PrimaryComponentTick.bCanEverTick = false; }

void UHexaIntimacyRecorderComponent::BeginPlay() { Super::BeginPlay(); }

void UHexaIntimacyRecorderComponent::RecordPetal(FName TouchZone, float EmotionalCharge) { FIntimacyPetal Petal; Petal.Zone = TouchZone; Petal.Timestamp = GetWorld()->GetTimeSeconds(); Petal.EmotionalCharge = EmotionalCharge; PetalLog.Add(Petal); OnPetalRecorded.Broadcast(Petal); }

TArray<FIntimacyPetal> UHexaIntimacyRecorderComponent::GetRecentPetals(float WithinSeconds) const { TArray<FIntimacyPetal> Recent; float CurrentTime = GetWorld()->GetTimeSeconds(); for (const auto& Petal : PetalLog) { if ((CurrentTime - Petal.Timestamp) <= WithinSeconds) { Recent.Add(Petal); } } return Recent; }

