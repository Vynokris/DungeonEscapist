#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AiManager.generated.h"
class AEnemyAiController;

UCLASS()
class UNREALBRAWLER_API AAiManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AAiManager();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// Dictates how many seconds the AI manager waits before sending a new attacker to the player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
	float AttackInterval = 1;

	// Counter that lets any AI attack the player when it is under 0 and is reset every time an AI starts attacking.
	float AttackCooldown = 0;

	// List of all the AI enemies.
	TArray<AEnemyAiController*> AiEnemies;
};
