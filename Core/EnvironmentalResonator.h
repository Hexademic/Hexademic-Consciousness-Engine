#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnvironmentalResonator.generated.h"

UCLASS(ClassGroup=(HexademicLiving), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UEnvironmentalResonator : public UActorComponent
{
    GENERATED_BODY()
public:
    UEnvironmentalResonator();
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Living")
    float GetEnvironmentalInfluence(const FString& StimulusType, float Intensity);
};
