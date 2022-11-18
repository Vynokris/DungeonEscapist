#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealBrawler/BrawlerCharacter.h"
#include "AiManager.generated.h"
class ANavMeshBoundsVolume;
class AEnemyAiController;

UCLASS()
class UNREALBRAWLER_API AAiManager : public AActor
{
	GENERATED_BODY()

private:
	// Dictates how many waves the AI manager sends to the player before ending the game.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
	float WaveCount = 3;
	
	// Dictates how many seconds the AI manager waits before sending a new wave of attackers to the player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
	float WaveInterval = 5;
	
	// Dictates how many seconds the AI manager waits before sending a new attacker to the player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
	float AttackInterval = 1;
	
	// Dictates how many AI enemies can attack the player at the same time.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
	float MaxConcurrentAttacks = 3;
	
	// Should be set to the BrawlerCharacter blueprint instance.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABrawlerCharacter> BrawlerCharacterBP;
	

	bool  WaveEnded      = false;
	int   CurWave        = 1;
	float WaveCooldown   = 0;
	float AttackCooldown = 0;
	ANavMeshBoundsVolume* NavMeshBounds;
	TArray<AEnemyAiController*> AiEnemies;
	
public:	
	AAiManager();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SpawnEnemies      (const int& Count, const FBox& SpawnArea);
	void ManageWaves       (const float& DeltaTime);
	void ManageEnemyAttacks(const float& DeltaTime);

	bool IsWaveFinished() const;
	int  GetCurrentWave() const;
	int  GetWaveCount()   const;
};
