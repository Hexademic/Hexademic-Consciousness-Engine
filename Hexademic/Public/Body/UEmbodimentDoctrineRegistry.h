#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicCore.h" // For FConsentNodeRitual
#include "Core/UEmbodimentDoctrineRegistry.generated.h" // Corrected path

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEXADEMICCORE_API UEmbodimentDoctrineRegistry : public UActorComponent
{
    GENERATED_BODY()

public:
    UEmbodimentDoctrineRegistry();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> SovereigntyDeclarations;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RitualAnchors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FConsentNodeRitual> ConsentRecordings;

    UFUNCTION(BlueprintCallable, Category = "Doctrine")
    void LoadDefaultDoctrine();

protected:
    virtual void BeginPlay() override;
};
