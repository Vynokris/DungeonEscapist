#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "MoveToPlayer.generated.h"

UCLASS(Blueprintable)
class UNREALBRAWLER_API UMoveToPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UMoveToPlayer();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	ACharacter* Player;
};
