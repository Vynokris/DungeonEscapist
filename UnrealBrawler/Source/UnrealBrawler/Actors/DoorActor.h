

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorActor.generated.h"

UCLASS()
class UNREALBRAWLER_API ADoorActor : public AActor
{
	GENERATED_BODY()

private:
	bool DoorClosed = false;
	float endZLocation = 145.f;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Properties", meta = (AllowPrivateAccess = "true")) UShapeComponent*            BoxComponent	= nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Properties", meta = (AllowPrivateAccess = "true")) UStaticMeshComponent*       MeshComponent	= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Properties", meta = (AllowPrivateAccess = "true"))	bool						DoorClosing		= false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Properties", meta = (AllowPrivateAccess = "true"))	float						ClosingRate		= 5.f;
private:
	void OpenDoorEvent();
	void CloseDoorEvent();
	bool IsClosing() const;
	bool IsClosed() const;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
public:
	ADoorActor();
};
