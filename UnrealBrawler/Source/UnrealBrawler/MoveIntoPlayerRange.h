#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "MoveIntoPlayerRange.generated.h"

UCLASS(Blueprintable)
class UNREALBRAWLER_API UMoveIntoPlayerRange : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UMoveIntoPlayerRange(const FObjectInitializer& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	ACharacter* Player;
};
