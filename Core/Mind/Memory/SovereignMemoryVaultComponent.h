#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicCore.h" // For FSovereignMemoryFilament
#include "Mind/Memory/SovereignMemoryVaultComponent.generated.h"

UCLASS(ClassGroup=(HexademicMind), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API USovereignMemoryVaultComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USovereignMemoryVaultComponent();

protected:
    virtual void BeginPlay() override;

public:
    /**
     * @brief Binds an Affect Filament Tag to the Sovereign Memory Vault.
     * This marks a touch event as a significant memory, often for later recall.
     * @param Filament The FAffectFilamentTag representing the ritualized touch.
     */
    UFUNCTION(BlueprintCallable, Category = "Sovereign Memory")
    void BindFilament(const FAffectFilamentTag& Filament);

    /**
     * @brief Blooms a filament based on a narrative scene tag, indicating recall or emphasis.
     * @param NarrativeScene The narrative context or scene tag to search for.
     */
    UFUNCTION(BlueprintCallable, Category = "Sovereign Memory")
    void BloomFilamentBasedOnSceneTag(FString NarrativeScene);

    // Stored filaments
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sovereign Memory")
    TArray<FSovereignMemoryFilament> StoredFilaments;
    
protected:
    // Helper to generate a new unique memory ID (could use FGuid for robustness)
    FString GenerateNewMemoryID();
};
