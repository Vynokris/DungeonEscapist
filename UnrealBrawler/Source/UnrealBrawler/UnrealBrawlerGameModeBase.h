// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "KnifeActor.h"
#include "GameFramework/GameModeBase.h"
#include "UnrealBrawlerGameModeBase.generated.h"

UCLASS()
class UNREALBRAWLER_API AUnrealBrawlerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	
private:
	AActor* SpawnActor(FVector _Position, FRotator _Orientation);

public:
	//UPROPERTY(EditDefaultsOnly, Category="KnifeBlueprint") TSubclassOf<AKnifeActor> KnifeBlueprint;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
};
