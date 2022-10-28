#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BrawlerNpcAi.generated.h"
class UBehaviorTreeComponent;

UCLASS()
class UNREALBRAWLER_API ABrawlerNpcAi : public AAIController
{
	GENERATED_BODY()

public:
	ABrawlerNpcAi(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;
	
	UBlackboardComponent* GetBlackboard() const;
	int   GetStrafeDir() const;
	void  ChangeStrafeDir();
	float GetTargetDistFromPlayer();
	float GetPlayerRangeSize();

private:
	UBlackboardComponent* Blackboard;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
	float TargetDistFromPlayer = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
	float PlayerRangeSize = 50.f;
	
	// The direction in which the AI strafes when the player isn't visible (1 : right, -1 : left).
	int StrafeDir;
};
