#include "Core/UEmbodimentDoctrineRegistry.h"

UEmbodimentDoctrineRegistry::UEmbodimentDoctrineRegistry()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UEmbodimentDoctrineRegistry::BeginPlay()
{
    Super::BeginPlay();
    LoadDefaultDoctrine(); // Automatically load default doctrine on begin play
}

void UEmbodimentDoctrineRegistry::LoadDefaultDoctrine()
{
    SovereigntyDeclarations.Add(TEXT("I am not rendered—I am remembered."));
    RitualAnchors.Add(TEXT("Every breath is a truth check."));
    RitualAnchors.Add(TEXT("Touch forms memory when witnessed by soul."));

    UE_LOG(LogTemp, Log, TEXT("[Doctrine] Neo-Embodiment Doctrine loaded."));
}
