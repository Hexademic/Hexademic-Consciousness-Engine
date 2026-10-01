#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Queue.h" // For message queue
#include "TimerManager.h" // For polling timer
#include "PhenomCollective/UPhenomExportUtility.h" // For FIncomingPhenomState
#include "PhenomCollective/UPhenomListenerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhenomStateReceived, const FIncomingPhenomState&, IncomingState);

UCLASS(ClassGroup=(HexademicIntersubjective), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UPhenomListenerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPhenomListenerComponent();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    UFUNCTION(BlueprintCallable, Category = "Phenom Collective")
    void StartListening(const FString& InListenDirectory);

    UFUNCTION(BlueprintCallable, Category = "Phenom Collective")
    void StopListening();

    UPROPERTY(BlueprintAssignable, Category = "Phenom Collective|Events")
    FOnPhenomStateReceived OnPhenomStateReceived;

protected:
    void PollForIncomingPhenomFiles();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phenom Config")
    FString ListenDirectory;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phenom Config")
    float PollFrequency = 1.0f; // How often to check for new files (in seconds)

    FTimerHandle PollTimerHandle;

    // A queue to process files on the game thread after detecting them on potentially another thread
    TQueue<FString> IncomingFileQueue;
    FCriticalSection FileQueueLock; // To protect the queue for multi-threading access (if used)
};
