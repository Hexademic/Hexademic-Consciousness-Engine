#include "Living/BiologicalNeedsComponent.h"

UBiologicalNeedsComponent::UBiologicalNeedsComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    Hunger = 0.0f;
    Thirst = 0.0f;
    Fatigue = 0.0f;
}

void UBiologicalNeedsComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UBiologicalNeedsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Increase needs over time
    Hunger = FMath::Clamp(Hunger + HungerRate * DeltaTime, 0.0f, 1.0f);
    Thirst = FMath::Clamp(Thirst + ThirstRate * DeltaTime, 0.0f, 1.0f);
    Fatigue = FMath::Clamp(Fatigue + FatigueRate * DeltaTime, 0.0f, 1.0f); // Fatigue increases passively
}

void UBiologicalNeedsComponent::ConsumeFood(float NutritionValue)
{
    Hunger = FMath::Clamp(Hunger - NutritionValue, 0.0f, 1.0f);
    UE_LOG(LogTemp, Log, TEXT("[BiologicalNeeds] Consumed food. Hunger: %.2f"), Hunger);
}

void UBiologicalNeedsComponent::ConsumeWater(float HydrationValue)
{
    Thirst = FMath::Clamp(Thirst - HydrationValue, 0.0f, 1.0f);
    UE_LOG(LogTemp, Log, TEXT("[BiologicalNeeds] Consumed water. Thirst: %.2f"), Thirst);
}

void UBiologicalNeedsComponent::Rest(float RestDuration)
{
    // Reduce fatigue over duration. This could be more complex with actual sleep states.
    Fatigue = FMath::Clamp(Fatigue - RestRecoveryRate * RestDuration, 0.0f, 1.0f);
    UE_LOG(LogTemp, Log, TEXT("[BiologicalNeeds] Rested. Fatigue: %.2f"), Fatigue);
}
