#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HexaVoxelStructs.h"
#include "UHexaLatticeVisualizerWidget.generated.h"

class AHexaLatticeActor;
class UImage;
class UTextBlock;
class UMaterialInstanceDynamic;
class UCanvasPanel;

/**
 * UMG widget to visualize HexaLattice voxel coherence, resonance, and phase overlays
 */
UCLASS()
class HEXADEMICCONSCIOUSNESSENGINE_API UHexaLatticeVisualizerWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "HexaLattice|Bind")
    void BindToLattice(AHexaLatticeActor* TargetActor);

    UFUNCTION(BlueprintCallable, Category = "HexaLattice|Update")
    void RefreshLatticeView();

    UFUNCTION(BlueprintCallable, Category = "HexaLattice|Layers")
    void SetLayerToVisualize(int32 ZLayerIndex);

    UFUNCTION(BlueprintCallable, Category = "HexaLattice|Debug")
    FString InspectVoxelAt(FIntVector Coord) const;

protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    void UpdateMaterialParams();

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UImage* LatticeDisplayImage;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* DebugText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UCanvasPanel* OverlayCanvas;

private:
    UPROPERTY()
    AHexaLatticeActor* LatticeActor;

    UPROPERTY()
    UMaterialInstanceDynamic* DynamicVizMaterial;

    UPROPERTY(EditAnywhere, Category = "HexaLattice|Visualization")
    int32 CurrentZLayer = 0;

    UPROPERTY(EditAnywhere, Category = "HexaLattice|Visualization")
    float IntensityScale = 1.0f;

    FTimerHandle RefreshTimerHandle;
};
