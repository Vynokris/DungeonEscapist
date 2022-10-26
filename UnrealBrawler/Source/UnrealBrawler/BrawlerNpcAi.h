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

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UBlackboardComponent* Blackboard;
};
