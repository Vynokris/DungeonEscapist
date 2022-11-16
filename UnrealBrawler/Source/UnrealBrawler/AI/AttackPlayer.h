#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "AttackPlayer.generated.h"

UCLASS(Blueprintable)
class UNREALBRAWLER_API UAttackPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UAttackPlayer();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	ACharacter* Player;
};
