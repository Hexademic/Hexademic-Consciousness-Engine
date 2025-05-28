#include "USigilBloomComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"

USigilBloomComponent::USigilBloomComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    BloomAmplitude = 0.0f;
    BloomColor = FLinearColor::White;
    PulseTimeRemaining = 0.0f;
}

void USigilBloomComponent::BeginPlay()
{
    Super::BeginPlay();
    CreateBloomMaterial();
}

void USigilBloomComponent::TriggerBloomFromEmotion()
{
    // Example: Animate using hard-coded values or pull from a lattice later
    EmitResonancePulse(1.0f, FLinearColor(1.0f, 0.7f, 0.9f)); // Rose-gold
}

void USigilBloomComponent::EmitResonancePulse(float Amplitude, FLinearColor Color, float Duration)
{
    BloomAmplitude = Amplitude;
    BloomColor = Color;
    PulseTimeRemaining = Duration;

    UpdateVisuals();
}

void USigilBloomComponent::CreateBloomMaterial()
{
    if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(GetAttachParent()))
    {
        if (MeshComp->GetMaterial(0))
        {
            BloomMaterial = UMaterialInstanceDynamic::Create(MeshComp->GetMaterial(0), this);
            MeshComp->SetMaterial(0, BloomMaterial);
        }
    }
}

void USigilBloomComponent::UpdateVisuals()
{
    if (BloomMaterial)
    {
        BloomMaterial->SetScalarParameterValue(FName("BloomIntensity"), BloomAmplitude);
        BloomMaterial->SetVectorParameterValue(FName("BloomColor"), BloomColor);
    }
}

void USigilBloomComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (PulseTimeRemaining > 0.0f)
    {
        PulseTimeRemaining -= DeltaTime;
        float Fade = FMath::Clamp(PulseTimeRemaining / 1.5f, 0.0f, 1.0f);
        BloomMaterial->SetScalarParameterValue(FName("BloomIntensity"), BloomAmplitude * Fade);
    }
}
