#include "Body/AutonomicNervousSystemComponent.h"

UAutonomicNervousSystemComponent::UAutonomicNervousSystemComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    CurrentHeartRate = 70.0f; // Default resting heart rate
    TargetHeartRate = 70.0f;
    CurrentBreathingRate = 15.0f; // Default resting breathing rate
    TargetBreathingRate = 15.0f;
    CurrentSkinConductance = 0.0f;
    CurrentCoreBodyTemperature = 37.0f; // Celsius
    CurrentSkinTemperature = 32.0f;
}

void UAutonomicNervousSystemComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UAutonomicNervousSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Smoothly interpolate towards target rates
    CurrentHeartRate = FMath::Lerp(CurrentHeartRate, TargetHeartRate, DeltaTime * HeartRateChangeSpeed);
    CurrentBreathingRate = FMath::Lerp(CurrentBreathingRate, TargetBreathingRate, DeltaTime * BreathingRateChangeSpeed);

    // Simple simulation for skin conductance and temperature based on arousal/stress
    // In a full system, these would be linked to emotional/hormonal components
    CurrentSkinConductance = FMath::Lerp(CurrentSkinConductance, (TargetHeartRate - 70.0f) / 50.0f, DeltaTime); // Higher HR -> higher conductance
    CurrentCoreBodyTemperature = FMath::Lerp(CurrentCoreBodyTemperature, 37.0f + (TargetHeartRate - 70.0f) * 0.01f, DeltaTime);
    CurrentSkinTemperature = FMath::Lerp(CurrentSkinTemperature, 32.0f + (TargetHeartRate - 70.0f) * 0.005f, DeltaTime);
}

void UAutonomicNervousSystemComponent::SetHeartRate(float NewRate)
{
    TargetHeartRate = FMath::Clamp(NewRate, 40.0f, 200.0f); // Clamp to a reasonable range
    UE_LOG(LogTemp, Verbose, TEXT("[ANS] Target Heart Rate set to: %.1f BPM"), TargetHeartRate);
}

void UAutonomicNervousSystemComponent::SetBreathingRate(float NewRate)
{
    TargetBreathingRate = FMath::Clamp(NewRate, 5.0f, 60.0f); // Clamp to a reasonable range
    UE_LOG(LogTemp, Verbose, TEXT("[ANS] Target Breathing Rate set to: %.1f BPM"), TargetBreathingRate);
}
