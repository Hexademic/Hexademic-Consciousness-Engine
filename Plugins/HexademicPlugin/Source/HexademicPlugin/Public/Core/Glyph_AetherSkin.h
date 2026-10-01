#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicCore.h" // For FAetherTouchPacket
#include "Glyph_AetherSkin.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEXADEMICCORE_API UGlyph_AetherSkin : public UActorComponent {
    GENERATED_BODY()
public:
    UGlyph_AetherSkin();
protected:
    virtual void BeginPlay() override;
public:
    UFUNCTION(BlueprintCallable, Category="AetherSkin")
    void SendHapticSignal(const FAetherTouchPacket& Packet);
    UFUNCTION(BlueprintCallable, Category="AetherSkin")
    void TriggerRegionFeedback(FString Region, float Intensity, float Duration);
private:
    void RouteToHardware(const FAetherTouchPacket& Packet);
    FString NormalizeRegion(FString Region);
};
