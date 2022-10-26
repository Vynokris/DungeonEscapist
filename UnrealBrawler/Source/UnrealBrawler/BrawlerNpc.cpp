#include "BrawlerNpc.h"

// Sets default values.
ABrawlerNpc::ABrawlerNpc()
{
 	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Get a random strafe direction.
    StrafeDir = FMath::RandRange(0, 1) == 0 ? 1 : -1;
}

// Called when the game starts or when spawned.
void ABrawlerNpc::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame.
void ABrawlerNpc::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input.
void ABrawlerNpc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Returns the AI's strafe direction (1 : right, -1 : left).
int ABrawlerNpc::GetStrafeDir() const
{
	return StrafeDir;
}

