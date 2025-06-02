#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicCore.h" // For FPackedHexaSigilNode
#include "Mind/RecursiveAwarenessComponent.h" // For IdentityRootFilaments
#include "Mind/SelfReflectionEngine.generated.h"

// Forward declaration for FEmotionalState
struct FEmotionalState; // Assuming this is defined in HexademicCore.h

// FEmotionalSigil: A simplified sigil for self-reflection
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FEmotionalSigil
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil")
    FString SigilID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil")
    FEmotionalState EmotionProfile;
};

UCLASS(ClassGroup=(HexademicMind), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API USelfReflectionEngine : public UActorComponent
{
    GENERATED_BODY()

public:
    USelfReflectionEngine();

protected:
    virtual void BeginPlay() override;

public:
    /**
     * @brief Crystallizes emotional sigils from the identified Identity Roots.
     * This represents the process of self-understanding and forming core emotional patterns.
     */
    UFUNCTION(BlueprintCallable, Category = "Self Reflection")
    void CrystallizeSigilFromIdentityRoots();

    // Reference to the Recursive Awareness Component
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<URecursiveAwarenessComponent> LinkedAwareness;

    // Active sigils representing crystallized self-understanding
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reflected Sigils")
    TArray<FEmotionalSigil> ActiveReflectionSigils;

protected:
    // Conceptual helper to retrieve an emotional vector associated with a memory ID
    FEmotionalState RetrieveEmotionVector(const FString& MemoryID) const;
};
