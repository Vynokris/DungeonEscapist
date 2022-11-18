#include "AiManager.h"

#include "../BrawlerCharacter.h"
#include "EnemyAiController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavMesh/NavMeshBoundsVolume.h"

AAiManager::AAiManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAiManager::BeginPlay()
{
	Super::BeginPlay();

	// Find the scene navigation mesh bounds.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "NavMeshBounds", FoundActors);
	if (FoundActors.Num() > 0)
		NavMeshBounds = Cast<ANavMeshBoundsVolume>(FoundActors[0]);

	// Find all enemies spawned by default in the level.
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyAiController::StaticClass(), FoundEnemies);
	for (int i = 0; i < FoundEnemies.Num(); i++) {
		AiEnemies.Push(Cast<AEnemyAiController>(FoundEnemies[i]));
	}
}

void AAiManager::Tick(float DeltaTime)
{
	Super::Tick       (DeltaTime);
	ManageWaves       (DeltaTime);
	ManageEnemyAttacks(DeltaTime);
}

void AAiManager::SpawnEnemies(const int& Count, const FBox& SpawnBox)
{
	for (int i = 0; i < Count; i++)
	{
		const FVector          RandPoint    = UKismetMathLibrary::RandomPointInBoundingBox(SpawnBox.GetCenter(), SpawnBox.GetExtent());
		UBehaviorTree*         BehaviorTree = nullptr;
		if (ABrawlerCharacter* NewEnemy     = Cast<ABrawlerCharacter>(UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), BrawlerCharacterBP, BehaviorTree, RandPoint)))
		{
			NewEnemy->BeginPlay();
			AiEnemies.Add(Cast<AEnemyAiController>(NewEnemy->GetController()));
		}
	}
}

void AAiManager::ManageWaves(const float& DeltaTime)
{
	// During a wave, check if all the enemies are dead and end the wave if that is the case.
	if (!WaveEnded)
	{
		WaveEnded = true;
		for (const AEnemyAiController* Ai : AiEnemies)
			if (const ABrawlerCharacter* Enemy = Cast<ABrawlerCharacter>(Ai->GetPawn()))
				if (!Enemy->IsDead())
					WaveEnded = false;
		if (WaveEnded)
			WaveCooldown = WaveInterval;
	}

	// When the wave is finished, wait for the wave cooldown to be over and start a new wave.
	else
	{
		if (WaveCooldown > 0) {
			WaveCooldown -= DeltaTime;
		}
		else if (CurWave < GetWaveCount()) {
			CurWave++;
			WaveCooldown = 0;
			WaveEnded    = false;
			AiEnemies.Empty();
			SpawnEnemies(FMath::RandRange(4, 6), NavMeshBounds->GetBounds().GetBox());
		}

		// End the game after the right amount of waves have been beaten.
		else {
			Cast<AUnrealBrawlerGameModeBase>(GetWorld()->GetAuthGameMode())->GetUserInterface()->ShowWinMenuEvent();
		}
	}
}

static bool CanEnemyAttack(const AEnemyAiController* Enemy)
{
	const ABrawlerCharacter* Character = Cast<ABrawlerCharacter>(Enemy->GetPawn());
	return  Character &&
		   !Character->IsDead() &&
			Enemy->IsInPlayerRange() &&
		   !Enemy->GetBlackboard()->GetValueAsBool("ShouldAttack") &&
		   !Character->IsAttacking();
}

void AAiManager::ManageEnemyAttacks(const float& DeltaTime)
{
	if (AttackCooldown <= 0)
	{
		// Get the number of enemies currently attacking the player.
		int ConcurrentAttacksCounter = 0;
		for (AEnemyAiController* Ai : AiEnemies)
			if (Ai->GetBlackboard()->GetValueAsBool("ShouldAttack"))
				ConcurrentAttacksCounter++;

		if (ConcurrentAttacksCounter < MaxConcurrentAttacks)
		{
			// Find a random enemy that is in range of the player and not attacking.
			int RandEnemyIdx = FMath::RandRange(0, AiEnemies.Num()-1);
			for (int i = 0; i < AiEnemies.Num() && !CanEnemyAttack(AiEnemies[RandEnemyIdx]); i++)
				RandEnemyIdx = (RandEnemyIdx + 1) % AiEnemies.Num();

			// Let the enemy start an attack.
			if (CanEnemyAttack(AiEnemies[RandEnemyIdx]))
			{
				AiEnemies[RandEnemyIdx]->GetBlackboard()->SetValueAsBool("ShouldAttack", true);
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

bool AAiManager::IsWaveFinished() const
{
	return WaveEnded;
}

int AAiManager::GetCurrentWave() const
{
	return CurWave;
}

int AAiManager::GetWaveCount() const
{
	return WaveCount;
}

