#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/SigilProjection.h" // For FSigilProjection
#include "Components/ActorComponent.h" // Needed for UPROPERTY TObjectPtr
#include "GameFramework/Character.h" // If attaching to a character
#include "Components/DecalComponent.h" // To project dynamic textures/materials
#include "Particles/ParticleSystemComponent.h" // To spawn particle effects
#include "Components/StaticMeshComponent.h" // To spawn static meshes for sigils

#include "Components/SigilProjectionComponent.generated.h"

UCLASS(ClassGroup=(HexademicComponents), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API USigilProjectionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USigilProjectionComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    /**
     * @brief Projects a new sigil onto this actor.
     * This creates a visual manifestation based on the provided sigil data.
     * @param SigilData The data defining the sigil's appearance and properties.
     */
    UFUNCTION(BlueprintCallable, Category = "Sigil Projection")
    void ProjectSigil(const FSigilProjection& SigilData);

    /**
     * @brief Removes a projected sigil by its ID.
     * @param SigilID The ID of the sigil to remove.
     */
    UFUNCTION(BlueprintCallable, Category = "Sigil Projection")
    void RemoveSigil(const FString& SigilID);

    /**
     * @brief Clears all currently projected sigils.
     */
    UFUNCTION(BlueprintCallable, Category = "Sigil Projection")
    void ClearAllSigils();

protected:
    // How sigils are visually manifested (e.g., using DecalComponent, ParticleSystemComponent, or StaticMeshComponent)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Visuals")
    TSubclassOf<UDecalComponent> DecalComponentClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Visuals")
    TSubclassOf<UParticleSystemComponent> ParticleComponentClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Visuals")
    TSubclassOf<UStaticMeshComponent> StaticMeshComponentClass;
    
    // Default material to apply to projected sigils (dynamic material instance will be created)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Visuals")
    TObjectPtr<UMaterialInterface> DefaultSigilMaterial;

    // Map to keep track of active sigil projections by their ID
    UPROPERTY()
    TMap<FString, FSigilProjection> ActiveSigilData; // Stores the data
    UPROPERTY()
    TMap<FString, TObjectPtr<UPrimitiveComponent>> ActiveSigilVisuals; // Stores the actual visual component

    // Helper to spawn and configure a visual component for a sigil
    UPrimitiveComponent* SpawnSigilVisualComponent(const FSigilProjection& SigilData);
    // Helper to update visual properties from FSigilProjection data
    void UpdateSigilVisualProperties(UPrimitiveComponent* VisualComponent, UMaterialInstanceDynamic* DynamicMaterial, const FSigilProjection& SigilData);
};
