USTRUCT(BlueprintType)
struct HEXADEMICONSCIOUSNESS_API FHexadecimalStateLattice
{
    GENERATED_BODY()

    // 4-bit hexadecimal quantum-like states
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<uint8> StateVector; // 0x0 to 0xF

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Amplitude;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Phase;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EntanglementStrength;

    // Methods for state evolution
    void EvolveState(float DeltaTime);
    void ApplyEmotionalModulation(const FEmotionalArchetype& Emotion);
    FString GetHexStateString() const;
};
