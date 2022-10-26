#include "BrawlerNpc.h"

// Sets default values.
ABrawlerNpc::ABrawlerNpc()
{
 	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned.
void ABrawlerNpc::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame.
void ABrawlerNpc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input.
void ABrawlerNpc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

