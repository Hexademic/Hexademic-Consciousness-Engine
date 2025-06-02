#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhenomCollective/UPhenomConstellationVisualizerComponent.generated.h"

UCLASS(ClassGroup=(HexademicIntersubjective), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UPhenomConstellationVisualizerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPhenomConstellationVisualizerComponent();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Phenom Collective|Visualizer")
    void LoadEchoLedgerFromFile(const FString& FilePath);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Phenom Collective|Visualizer")
    TArray<FString> GetVisualizedEchoEntries() const { return VisualizedEchoEntries; }

protected:
    // This would likely store parsed data from the ledger to drive visual effects
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Constellation Data")
    TArray<FString> VisualizedEchoEntries; // Example: Stores raw lines from ledger

    // Perhaps a UStaticMeshComponent or UParticleSystemComponent for the actual visualization
    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual Components")
    // TObjectPtr<UStaticMeshComponent> ConstellationMesh;
};
