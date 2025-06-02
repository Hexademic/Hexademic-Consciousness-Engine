#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IncrementalPersister.generated.h"

UCLASS(ClassGroup=(HexademicLiving), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UIncrementalPersister : public UActorComponent
{
    GENERATED_BODY()
public:
    UIncrementalPersister();
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Living")
    void PersistState(const FString& StateData);
};
