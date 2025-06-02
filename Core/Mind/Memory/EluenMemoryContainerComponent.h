#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.
#include "Mind/Memory/EluenMemoryContainerComponent.generated.h"

UCLASS(ClassGroup=(HexademicMind), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UEluenMemoryContainerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEluenMemoryContainerComponent();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Memory")
    void StoreMemory(const FString& MemoryID, const FString& MemoryContext);

    UFUNCTION(BlueprintCallable, Category = "Memory")
    bool RecallMemory(const FString& MemoryID, FString& OutMemoryContext) const;

    UFUNCTION(BlueprintCallable, Category = "Memory")
    void ForgetMemory(const FString& MemoryID);
    
    // Placeholder for a simple memory storage. In a real system, this would be more complex.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Memory")
    TMap<FString, FString> StoredMemories;
};
