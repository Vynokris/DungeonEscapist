#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MoveAwayFromPlayer.generated.h"

UCLASS(Blueprintable)
class UNREALBRAWLER_API UMoveAwayFromPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UMoveAwayFromPlayer();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	ACharacter* Player;
};
