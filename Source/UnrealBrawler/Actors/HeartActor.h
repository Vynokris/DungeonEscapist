#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeartActor.generated.h"

UCLASS()
class UNREALBRAWLER_API AHeartActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trophee Properties", meta = (AllowPrivateAccess = "true")) UShapeComponent*      BoxComponent  = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trophee Properties", meta = (AllowPrivateAccess = "true")) UStaticMeshComponent* MeshComponent = nullptr;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	AHeartActor();
	
	UStaticMeshComponent* GetMesh() const;
};
