#include "AiManager.h"
#include "../BrawlerCharacter.h"
#include "EnemyAiController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/TriggerBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "UnrealBrawler/BrawlerInstance.h"
#include "UnrealBrawler/Utils/DebugUtils.h"

AAiManager::AAiManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAiManager::BeginPlay()
{
    Super::BeginPlay();

    if (EntranceTrigger)
        EntranceTrigger->OnActorEndOverlap.AddDynamic(this, &AAiManager::StartWaves);

    // Find the scene navigation mesh bounds.
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), "NavMeshBounds", FoundActors);
    if (FoundActors.Num() > 0)
        NavMeshBounds = Cast<ANavMeshBoundsVolume>(FoundActors[0]);

    // Find all enemies spawned by default in the level.
    TArray<AActor*> FoundEnemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyAiController::StaticClass(), FoundEnemies);
    for (int i = 0; i < FoundEnemies.Num(); i++) {
        AiEnemies.Add(Cast<AEnemyAiController>(FoundEnemies[i]));
    }
}

void AAiManager::Tick(float DeltaTime)
{
    Super::Tick       (DeltaTime);
    if (WavesStarted)
    {
        ManageWaves       (DeltaTime);
        ManageEnemyAttacks(DeltaTime);
    }
}

void AAiManager::StartWaves(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor->Tags.Contains("Player"))
    {
        WavesStarted = true;
        if (EntranceTrigger)
            EntranceTrigger->OnActorEndOverlap.RemoveDynamic(this, &AAiManager::StartWaves);
    }
}

void AAiManager::SpawnEnemies(const int& Count, const FBox& SpawnArea)
{
    for (int i = 0; i < Count; i++)
    {
        // Find a random point on the navmesh and spawn an enemy there.
        FVector            RandPoint = UKismetMathLibrary::RandomPointInBoundingBox(SpawnArea.GetCenter(), SpawnArea.GetExtent()); RandPoint.Z = -7;
        ABrawlerCharacter* NewEnemy  = Cast<ABrawlerCharacter>(GetWorld()->SpawnActor<APawn>(BrawlerCharacterBP, RandPoint, FRotator::ZeroRotator));
        if (IsValid(NewEnemy))
        {
            NewEnemy->SpawnDefaultController();
            if (IsValid(NewEnemy->GetController()))
                AiEnemies.Add(Cast<AEnemyAiController>(NewEnemy->GetController()));
            else
                NewEnemy->Destroy();
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
        else if (CurWave < WaveCount) {
            CurWave++;
            WaveCooldown = 0;
            WaveEnded    = false;
            AiEnemies.Empty();
            Debug("Starting wave %d", CurWave);
            SpawnEnemies(FMath::RandRange(4, 6), NavMeshBounds->GetBounds().GetBox());
        }

        // End the game after the right amount of waves have been beaten.
        else {
            CastChecked<UBrawlerInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->SetGameWin(true);
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

