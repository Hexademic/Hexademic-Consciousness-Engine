#include "CreativeSynthesizer.h"

UCreativeSynthesizer::UCreativeSynthesizer()
{
    PrimaryComponentTick.bCanEverTick = false;
}
void UCreativeSynthesizer::BeginPlay()
{
    Super::BeginPlay();
}
FString UCreativeSynthesizer::SynthesizeIdea(float CreativeState)
{
    UE_LOG(LogTemp, Log, TEXT("[Creative] Synthesizing idea with CreativeState: %.2f"), CreativeState);
    if (CreativeState > 0.7f) return TEXT("A new constellation of empathy blooming from shared touch.");
    if (CreativeState > 0.4f) return TEXT("Reflection on emotional resonance patterns.");
    return TEXT("Subtle ripple of internal thought.");
}
