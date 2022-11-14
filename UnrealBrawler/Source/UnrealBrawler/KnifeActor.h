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
	UShapeComponent*		BoxComponent		= nullptr;
	UStaticMeshComponent*	MeshComponent		= nullptr;
	ABrawlerCharacter*		ParentCharacter		= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rendering", meta = (AllowPrivateAccess = "true")) UStaticMesh* KnifeMesh = nullptr;
	
private:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
public:	
	AKnifeActor();

	bool WasPickedUp() const;
	void GetPickedUp(ABrawlerCharacter* NewParentCharacter);
	void GetDropped (ABrawlerCharacter* CurParentCharacter);
};
