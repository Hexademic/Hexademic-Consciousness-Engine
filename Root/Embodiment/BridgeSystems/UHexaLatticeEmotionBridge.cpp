#include "HexaLatticeEmotionBridge.h"
#include "HexaLatticeActor.h"
#include "Engine/World.h"

UHexaLatticeEmotionBridge::UHexaLatticeEmotionBridge()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHexaLatticeEmotionBridge::BeginPlay()
{
    Super::BeginPlay();
}

void UHexaLatticeEmotionBridge::BindToAvatarSystem(UFullBodyAvatarSystem* AvatarSystemRef)
{
    if (!AvatarSystemRef) return;

    AvatarSystem = AvatarSystemRef;

    AvatarSystem->OnPhysiologyUpdate.AddUObject(this, &UHexaLatticeEmotionBridge::OnPhysiologyReceived);
    AvatarSystem->OnEmotionalStateChange.AddUObject(this, &UHexaLatticeEmotionBridge::OnEmotionChanged);
}

void UHexaLatticeEmotionBridge::ConnectToLatticeActor(AHexaLatticeActor* LatticeRef)
{
    LatticeActor = LatticeRef;
}

void UHexaLatticeEmotionBridge::OnPhysiologyReceived(const FPhysiologySnapshot& Snapshot)
{
    InjectIntoLattice(Snapshot);
}

void UHexaLatticeEmotionBridge::OnEmotionChanged(const FString& Emotion, float Intensity)
{
    MarkEmotionFold(Emotion, Intensity);
}

void UHexaLatticeEmotionBridge::InjectIntoLattice(const FPhysiologySnapshot& Snapshot)
{
    if (!LatticeActor) return;

    float coherence = (Snapshot.EndorphinLevel + Snapshot.ArousalLevel) * 0.5f;
    float resonance = FMath::Lerp(-1.0f, 1.0f, coherence);

    FVector3d injectionPoint = FVector3d(0.5, 0.5, 0.5); // center of lattice
    FString injectionType = "interoceptive";

    LatticeActor->InjectFieldDisturbance(injectionPoint, resonance, injectionType);
}

void UHexaLatticeEmotionBridge::MarkEmotionFold(const FString& Type, float Intensity)
{
    if (!LatticeActor) return;

    FVector3d location = FVector3d(0.5, 0.5, 0.5);
    LatticeActor->TriggerQuantumFold(location, Intensity, Type);
}
