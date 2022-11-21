#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorActor.generated.h"
class ATriggerBox;

UCLASS()
class UNREALBRAWLER_API ADoorActor : public AActor
{
	GENERATED_BODY()

private:
	bool  DoorMoving      = false;
	float ZLocationOpen   = 0;
	float ZLocationClosed = 0;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Properties", meta = (AllowPrivateAccess = "true")) UStaticMeshComponent* MeshComponent	  = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Properties", meta = (AllowPrivateAccess = "true"))	bool                  DoorClosed      = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Properties", meta = (AllowPrivateAccess = "true"))	bool                  TriggerOnce     = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Properties", meta = (AllowPrivateAccess = "true"))	float                 ClosingRate     = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Properties", meta = (AllowPrivateAccess = "true")) ATriggerBox*          EntranceTrigger = nullptr;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION()
	void OnTrigger(AActor* OverlappedActor, AActor* OtherActor);
	
public:
	ADoorActor();
	
	UFUNCTION(BlueprintCallable) void MoveDoorEvent();
	UFUNCTION(BlueprintCallable) bool IsMoving() const;
	UFUNCTION(BlueprintCallable) bool IsClosed() const;
};
