#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "StayInPlayerRange.generated.h"

UCLASS(Blueprintable)
class UNREALBRAWLER_API UStayInPlayerRange : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UStayInPlayerRange();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	ACharacter* Player;
};
