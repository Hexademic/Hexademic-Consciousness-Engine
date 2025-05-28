// HexademicIntegration.cpp // Implements logic for bridging avatar physiology with Hexademic field states
#include "HexademicIntegration.h" #include "FullBodyAvatarSystem.h" #include "Kismet/GameplayStatics.h" #include "HexademicVisualizerComponent.h" #include "Engine/World.h"
UHexademicIntegration::UHexademicIntegration() { PrimaryComponentTick.bCanEverTick = true; }
void UHexademicIntegration::BeginPlay() { Super::BeginPlay();
AvatarSystem = GetOwner()->FindComponentByClass<UFullBodyAvatarSystem>(); if (!AvatarSystem) { UE_LOG(LogTemp, Warning, TEXT("HexademicIntegration: FullBodyAvatarSystem not found.")); } // Bind to physiology update if (AvatarSystem) { AvatarSystem->OnPhysiologyUpdate.AddUObject(this, &UHexademicIntegration::InjectPhysiologyToLattice); AvatarSystem->OnEmotionalStateChange.AddUObject(this, &UHexademicIntegration::InjectEmotionToLattice); } // Find Visualizer in world (could be abstracted later) for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr) { UHexademicVisualizerComponent* FoundVis = ActorItr->FindComponentByClass<UHexademicVisualizerComponent>(); if (FoundVis) { Visualizer = FoundVis; break; } } if (!Visualizer) { UE_LOG(LogTemp, Warning, TEXT("HexademicIntegration: No HexademicVisualizerComponent found in world.")); } 
}
void UHexademicIntegration::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) { Super::TickComponent(DeltaTime, TickType, ThisTickFunction); // Optional: periodic passive updates or feedback visualizations }
void UHexademicIntegration::InjectPhysiologyToLattice(const FPhysiologySnapshot& Snapshot) { if (!Visualizer) return;
FVector Position = GetOwner()->GetActorLocation(); float Intensity = (Snapshot.ArousalLevel + Snapshot.EndorphinLevel) * 0.5f; FString FoldType = "interoceptive"; if (Snapshot.CortisolLevel > 0.6f) FoldType = "conflict"; Visualizer->InjectQuantumFold(Position, Intensity, FoldType); UE_LOG(LogTemp, Log, TEXT("Injected physiology to lattice: %s, Intensity %.2f"), *FoldType, Intensity); 
}
void UHexademicIntegration::InjectEmotionToLattice(const FString& Emotion, float Intensity) { if (!Visualizer) return;
FVector Position = GetOwner()->GetActorLocation(); FString FoldType = "emotional"; Visualizer->InjectQuantumFold(Position, Intensity, FoldType); UE_LOG(LogTemp, Log, TEXT("Injected emotion '%s' to lattice with intensity %.2f"), *Emotion, Intensity); 
}
