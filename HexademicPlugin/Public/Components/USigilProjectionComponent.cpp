UCLASS(ClassGroup=(HexademicConsciousness), meta=(BlueprintSpawnableComponent))
class HEXADEMICONSCIOUSNESS_API USigilProjectionComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    // Visual representation of consciousness state
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Projection")
    UMaterialInterface* SigilMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Projection")
    UStaticMeshComponent* SigilMesh;

    // Dynamic material parameters for real-time visualization
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Projection")
    FName EmotionalColorParameter = "EmotionalColor";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Projection")
    FName ConsciousnessIntensityParameter = "ConsciousnessIntensity";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Projection")
    FName HexStateParameter = "HexState";

    // Methods
    UFUNCTION(BlueprintCallable, Category = "Sigil Projection")
    void UpdateSigilVisualization(const FHexadecimalStateLattice& StateLattice,
                                 const TMap<EEmotionalArchetype, float>& EmotionalState);

    UFUNCTION(BlueprintCallable, Category = "Sigil Projection")
    void GenerateResonanceMap();
};
