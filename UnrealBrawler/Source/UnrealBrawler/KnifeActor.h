

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "KnifeActor.generated.h"

UCLASS()
class UNREALBRAWLER_API AKnifeActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere) USceneComponent* KnifeRoot;
	UPROPERTY(EditAnywhere) UStaticMeshComponent* KnifeMesh;
	UPROPERTY(EditAnywhere) UShapeComponent* KnifeBox;
	
private:
	void OnHit(UPrimitiveComponent* _HitComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _FromSweep, const FHitResult& _Hit);
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	
public:	
	AKnifeActor();
};
