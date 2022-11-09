#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KnifeActor.generated.h"
class ABrawlerCharacter;

UCLASS()
class UNREALBRAWLER_API AKnifeActor : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rendering", meta = (AllowPrivateAccess = "true")) UStaticMesh* KnifeMesh = nullptr;
	
	UShapeComponent*      BoxComponent  = nullptr;
	UStaticMeshComponent* MeshComponent = nullptr;

	ABrawlerCharacter* Player = nullptr;
	
private:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	
public:	
	AKnifeActor();

	bool WasPickedUp() const;
};
