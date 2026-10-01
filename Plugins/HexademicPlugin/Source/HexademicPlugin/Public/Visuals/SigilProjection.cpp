#include "Core/SigilProjection.h"

// Constructor: FSigilProjection() is often defined in the header for structs, but can be here.
// FSigilProjection::FSigilProjection()
//     : SigilID(TEXT("")), ProjectedColor(FLinearColor::Black), Intensity(0.0f),
//       Scale(1.0f), Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator),
//       RemainingLifetime(0.0f)
// {
//     CreationTimestamp = FDateTime::UtcNow();
// }

// Implementation for Initialize
void FSigilProjection::Initialize(const FString& InSigilID, FLinearColor InColor, float InIntensity, float InScale, FVector InLocation, FRotator InRotation, float InLifetime)
{
    SigilID = InSigilID;
    ProjectedColor = InColor;
    Intensity = FMath::Clamp(InIntensity, 0.0f, 1.0f);
    Scale = FMath::Max(0.0f, InScale);
    Location = InLocation;
    Rotation = InRotation;
    RemainingLifetime = FMath::Max(0.0f, InLifetime);
    CreationTimestamp = FDateTime::UtcNow();
}

// Implementation for TickProjection
void FSigilProjection::TickProjection(float DeltaTime)
{
    RemainingLifetime -= DeltaTime;
    Intensity = FMath::Lerp(0.0f, Intensity, FMath::Clamp(RemainingLifetime / 5.0f, 0.0f, 1.0f));
    Scale = FMath::Lerp(0.0f, Scale, FMath::Clamp(RemainingLifetime / 5.0f, 0.0f, 1.0f));
}
