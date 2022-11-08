#include "BrawlerNpc.h"

ABrawlerNpc::ABrawlerNpc()
{
	PrimaryActorTick.bCanEverTick = true;

	damageToApply = 1;
	type = Type::TANK;
}

void ABrawlerNpc::BeginPlay()
{
	Super::BeginPlay();
}

void ABrawlerNpc::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABrawlerNpc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

int ABrawlerNpc::GetDamage()
{
	return damageToApply;
}

ABrawlerNpc::Type ABrawlerNpc::GetType()
{
	return type;
}



