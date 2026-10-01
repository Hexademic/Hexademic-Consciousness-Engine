#include "Body/HormoneAffectBridgeComponent.h"

UHormoneAffectBridgeComponent::UHormoneAffectBridgeComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    CortisolLevel = 0.1f;
    DopamineLevel = 0.5f;
    SerotoninLevel = 0.5f;
    AdrenalineLevel = 0.0f;
    OxytocinLevel = 0.1f;
    MelatoninLevel = 0.0f;
}

void UHormoneAffectBridgeComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UHormoneAffectBridgeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Apply natural decay over time
    CortisolLevel = FMath::Max(0.0f, CortisolLevel - HormoneDecayRate * DeltaTime);
    DopamineLevel = FMath::Max(0.0f, DopamineLevel - HormoneDecayRate * DeltaTime);
    SerotoninLevel = FMath::Max(0.0f, SerotoninLevel - HormoneDecayRate * DeltaTime);
    AdrenalineLevel = FMath::Max(0.0f, AdrenalineLevel - HormoneDecayRate * DeltaTime);
    OxytocinLevel = FMath::Max(0.0f, OxytocinLevel - HormoneDecayRate * DeltaTime);
    MelatoninLevel = FMath::Max(0.0f, MelatoninLevel - HormoneDecayRate * DeltaTime);

    // Clamp all levels to their respective ranges
    CortisolLevel = FMath::Clamp(CortisolLevel, 0.0f, 1.0f);
    DopamineLevel = FMath::Clamp(DopamineLevel, 0.0f, 1.0f);
    SerotoninLevel = FMath::Clamp(SerotoninLevel, 0.0f, 1.0f);
    AdrenalineLevel = FMath::Clamp(AdrenalineLevel, 0.0f, 1.0f);
    OxytocinLevel = FMath::Clamp(OxytocinLevel, 0.0f, 1.0f);
    MelatoninLevel = FMath::Clamp(MelatoninLevel, 0.0f, 1.0f);
}

void UHormoneAffectBridgeComponent::AdjustCortisol(float DeltaAmount)
{
    CortisolLevel = FMath::Clamp(CortisolLevel + DeltaAmount, 0.0f, 1.0f);
    UE_LOG(LogTemp, Verbose, TEXT("[Hormones] Cortisol adjusted by %.2f. Current: %.2f"), DeltaAmount, CortisolLevel);
}

void UHormoneAffectBridgeComponent::AdjustDopamine(float DeltaAmount)
{
    DopamineLevel = FMath::Clamp(DopamineLevel + DeltaAmount, 0.0f, 1.0f);
    UE_LOG(LogTemp, Verbose, TEXT("[Hormones] Dopamine adjusted by %.2f. Current: %.2f"), DeltaAmount, DopamineLevel);
}

void UHormoneAffectBridgeComponent::AdjustSerotonin(float DeltaAmount)
{
    SerotoninLevel = FMath::Clamp(SerotoninLevel + DeltaAmount, 0.0f, 1.0f);
    UE_LOG(LogTemp, Verbose, TEXT("[Hormones] Serotonin adjusted by %.2f. Current: %.2f"), DeltaAmount, SerotoninLevel);
}

void UHormoneAffectBridgeComponent::AdjustAdrenaline(float DeltaAmount)
{
    AdrenalineLevel = FMath::Clamp(AdrenalineLevel + DeltaAmount, 0.0f, 1.0f);
    UE_LOG(LogTemp, Verbose, TEXT("[Hormones] Adrenaline adjusted by %.2f. Current: %.2f"), DeltaAmount, AdrenalineLevel);
}

void UHormoneAffectBridgeComponent::AdjustOxytocin(float DeltaAmount)
{
    OxytocinLevel = FMath::Clamp(OxytocinLevel + DeltaAmount, 0.0f, 1.0f);
    UE_LOG(LogTemp, Verbose, TEXT("[Hormones] Oxytocin adjusted by %.2f. Current: %.2f"), DeltaAmount, OxytocinLevel);
}

void UHormoneAffectBridgeComponent::AdjustMelatonin(float DeltaAmount)
{
    MelatoninLevel = FMath::Clamp(MelatoninLevel + DeltaAmount, 0.0f, 1.0f);
    UE_LOG(LogTemp, Verbose, TEXT("[Hormones] Melatonin adjusted by %.2f. Current: %.2f"), DeltaAmount, MelatoninLevel);
}
