#include "UHexaLatticeVisualizerWidget.h"
#include "HexaLatticeActor.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/TextureRenderTarget2D.h"
#include "TimerManager.h"

void UHexaLatticeVisualizerWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (LatticeDisplayImage && LatticeActor)
    {
        auto BaseMaterial = LatticeDisplayImage->GetDynamicMaterial();
        if (BaseMaterial)
        {
            DynamicVizMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
            LatticeDisplayImage->SetBrushFromMaterial(DynamicVizMaterial);
        }
    }

    GetWorld()->GetTimerManager().SetTimer(
        RefreshTimerHandle, this, &UHexaLatticeVisualizerWidget::RefreshLatticeView, 0.2f, true);
}

void UHexaLatticeVisualizerWidget::BindToLattice(AHexaLatticeActor* TargetActor)
{
    LatticeActor = TargetActor;
    RefreshLatticeView();
}

void UHexaLatticeVisualizerWidget::RefreshLatticeView()
{
    if (!LatticeActor || !DynamicVizMaterial) return;

    UTextureRenderTarget2D* LayerTexture = LatticeActor->GetRenderTargetForZLayer(CurrentZLayer);
    if (LayerTexture)
    {
        DynamicVizMaterial->SetTextureParameterValue("LayerRT", LayerTexture);
        DynamicVizMaterial->SetScalarParameterValue("IntensityScale", IntensityScale);
    }

    if (DebugText)
    {
        DebugText->SetText(FText::FromString(
            FString::Printf(TEXT("Coherence: %.3f"), LatticeActor->GetGlobalCoherence())));
    }
}

void UHexaLatticeVisualizerWidget::SetLayerToVisualize(int32 ZLayerIndex)
{
    CurrentZLayer = ZLayerIndex;
    RefreshLatticeView();
}

FString UHexaLatticeVisualizerWidget::InspectVoxelAt(FIntVector Coord) const
{
    if (!LatticeActor) return TEXT("No Actor Bound");
    FHexaVoxelState Voxel = LatticeActor->GetVoxelAt(Coord);
    return FString::Printf(TEXT("Q=%.2f+%.2fi C=%.2f R=%.2f"),
        Voxel.qumode.X, Voxel.qumode.Y, Voxel.coherence, Voxel.resonance);
}
