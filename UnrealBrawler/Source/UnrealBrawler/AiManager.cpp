#include "AiManager.h"

#include "BrawlerCharacter.h"
#include "EnemyAiController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"

AAiManager::AAiManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAiManager::BeginPlay()
{
	Super::BeginPlay();

	// Find all enemies spawned by default in the level.
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyAiController::StaticClass(), FoundEnemies);
	for (int i = 0; i < FoundEnemies.Num(); i++) {
		AiEnemies.Push(Cast<AEnemyAiController>(FoundEnemies[i]));
	}
}

static bool CanEnemyAttack(const AEnemyAiController* Enemy)
{
	const ABrawlerCharacter* Character = Cast<ABrawlerCharacter>(Enemy->GetPawn());
	return  Character &&
		   !Character->IsDead() &&
		    Enemy->IsInPlayerRange() &&
		   !Enemy->GetBlackboard()->GetValueAsBool("Attacking");
}

void AAiManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AttackCooldown <= 0)
	{
		int ConcurrentAttacksCounter = 0;
		for (AEnemyAiController* Ai : AiEnemies)
			if (Ai->GetBlackboard()->GetValueAsBool("Attacking"))
				ConcurrentAttacksCounter++;

		if (ConcurrentAttacksCounter < MaxConcurrentAttacks)
		{
			// Find a random enemy that is in range of the player and not attacking.
			int RandEnemyIdx = FMath::RandRange(0, AiEnemies.Num()-1);
			for (int j = 0; j < AiEnemies.Num() && !CanEnemyAttack(AiEnemies[RandEnemyIdx]); j++)
				RandEnemyIdx = (RandEnemyIdx + 1) % AiEnemies.Num();

			// Let the enemy start an attack.
			if (CanEnemyAttack(AiEnemies[RandEnemyIdx]))
			{
				AiEnemies[RandEnemyIdx]->GetBlackboard()->SetValueAsBool("Attacking", true);
				AttackCooldown = AttackInterval;
			}
		}
		else
		{
			AttackCooldown = AttackInterval;
		}
	}
	else
	{
		AttackCooldown -= DeltaTime;
	}
}

