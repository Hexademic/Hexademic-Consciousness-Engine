#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HeartbeatConsolidator.generated.h"

UCLASS(ClassGroup=(HexademicLiving), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UHeartbeatConsolidator : public UActorComponent
{
    GENERATED_BODY()
public:
    UHeartbeatConsolidator();
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Living")
    float GetHeartbeatRate() const;
};
