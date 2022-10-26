#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "GetInRangeOfPlayer.generated.h"

UCLASS(Blueprintable)
class UNREALBRAWLER_API UGetInRangeOfPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UGetInRangeOfPlayer(const FObjectInitializer& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	ACharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
	float TargetDistFromPlayer = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
	float RangeSize = 50.f;
};
