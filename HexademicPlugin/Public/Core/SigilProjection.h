#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h" // Needed for USTRUCT
#include "Math/Color.h" // For FLinearColor
#include "Math/Vector.h" // For FVector
#include "Misc/DateTime.h" // For FDateTime

/**
 * @brief Represents the data for a visual Sigil Projection, driven by consciousness states.
 * Sigils are dynamic glyphs that manifest from emotional and consciousness states.
 */
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FSigilProjection
{
    GENERATED_BODY()

    // Unique identifier for this specific sigil projection instance.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Projection")
    FString SigilID;

    // The visual color of the sigil, reflecting its emotional or energetic signature.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Projection")
    FLinearColor ProjectedColor;

    // The intensity or brightness of the projection (0.0 to 1.0).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Projection")
    float Intensity;

    // The scale of the sigil in world units or relative to the entity.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Projection")
    float Scale;

    // The world location where the sigil is projected.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Projection")
    FVector Location;

    // The rotational alignment of the sigil.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Projection")
    FRotator Rotation;

    // The lifetime of the sigil projection, allowing it to fade or disappear over time.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Projection")
    float RemainingLifetime;

    // Timestamp when this sigil was first projected.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Projection")
    FDateTime CreationTimestamp;

    // Default Constructor
    FSigilProjection()
        : SigilID(TEXT("")), ProjectedColor(FLinearColor::Black), Intensity(0.0f),
          Scale(1.0f), Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator),
          RemainingLifetime(0.0f)
    {
        CreationTimestamp = FDateTime::UtcNow();
    }

    /**
     * @brief Initializes the sigil projection with core properties.
     * @param InSigilID Unique ID for the sigil.
     * @param InColor The visual color.
     * @param InIntensity The intensity.
     * @param InScale The scale.
     * @param InLocation The world location.
     * @param InRotation The rotation.
     * @param InLifetime The initial lifetime.
     */
    UFUNCTION(BlueprintCallable, Category = "Sigil Projection")
    void Initialize(const FString& InSigilID, FLinearColor InColor, float InIntensity, float InScale, FVector InLocation, FRotator InRotation, float InLifetime)
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

    /**
     * @brief Updates the sigil's state over time, typically reducing its lifetime.
     * @param DeltaTime The time elapsed since the last update.
     */
    UFUNCTION(BlueprintCallable, Category = "Sigil Projection")
    void TickProjection(float DeltaTime)
    {
        RemainingLifetime -= DeltaTime;
        // Optionally, fade intensity or scale based on remaining lifetime
        Intensity = FMath::Lerp(0.0f, Intensity, FMath::Clamp(RemainingLifetime / 5.0f, 0.0f, 1.0f)); // Fade over last 5 seconds
        Scale = FMath::Lerp(0.0f, Scale, FMath::Clamp(RemainingLifetime / 5.0f, 0.0f, 1.0f));
    }
};
