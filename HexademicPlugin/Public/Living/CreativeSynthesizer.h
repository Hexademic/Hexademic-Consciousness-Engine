#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CreativeSynthesizer.generated.h"

UCLASS(ClassGroup=(HexademicLiving), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UCreativeSynthesizer : public UActorComponent
{
    GENERATED_BODY()
public:
    UCreativeSynthesizer();
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Living")
    FString SynthesizeIdea(float CreativeState);
};
