// Copyright Epic Games, Inc. All Rights Reserved.


#include "DebugUtils.h"

#include "UnrealBrawlerGameModeBase.h"

#include "KnifeActor.h"

void AUnrealBrawlerGameModeBase()
{
    
}

void AUnrealBrawlerGameModeBase::BeginPlay()
{
    //SpawnActor(FVector(0, 60, 110.250), FRotator(0));
    
    Super::BeginPlay();
}

void AUnrealBrawlerGameModeBase::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);
    
}

AActor* AUnrealBrawlerGameModeBase::SpawnActor(FVector _Position, FRotator _Orientation)
{
    //FActorSpawnParameters SpawnParameters;
    //SpawnParameters.Template = Cast<AKnifeActor>(KnifeBlueprint);
    //return GetWorld()->SpawnActor<AActor>(KnifeBlueprint, _Position, _Orientation, SpawnParameters);
    return nullptr;
}