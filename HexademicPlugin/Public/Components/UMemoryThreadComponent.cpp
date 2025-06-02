USTRUCT(BlueprintType)
struct HEXADEMICONSCIOUSNESS_API FEmotionalMemory
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EEmotionalArchetype, float> EmotionalSnapshot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FHexadecimalStateLattice ConsciousnessSnapshot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ContextDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResonanceStrength;
};

UCLASS(ClassGroup=(HexademicConsciousness), meta=(BlueprintSpawnableComponent))
class HEXADEMICONSCIOUSNESS_API UMemoryThreadComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Memory storage
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Threads")
    TArray<FEmotionalMemory> MemoryThreads;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Configuration")
    int32 MaxMemoryThreads = 1000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory Configuration")
    float MemoryDecayRate = 0.001f;

    // Methods
    UFUNCTION(BlueprintCallable, Category = "Memory Threads")
    void RecordMemory(const FEmotionalMemory& Memory);

    UFUNCTION(BlueprintCallable, Category = "Memory Threads")
    TArray<FEmotionalMemory> RetrieveRelevantMemories(EEmotionalArchetype QueryEmotion, 
                                                     float RelevanceThreshold = 0.5f);

    UFUNCTION(BlueprintCallable, Category = "Memory Threads")
    void ExportMemoriesToJSON(const FString& FilePath);
};
