#include "AttackPlayer.h"

#include "BrawlerNpcAi.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UAttackPlayer::UAttackPlayer()
{
    NodeName = "Attack player";
}

EBTNodeResult::Type UAttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Get the player and AI controller.
    if (!Player) {
        Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    }
    ABrawlerNpcAi*        Ai           = Cast<ABrawlerNpcAi>(OwnerComp.GetAIOwner());
    UBlackboardComponent* AiBlackboard = Ai->GetBlackboard();

    // Get the player and AI locations as well as their distance.
    const FVector PlayerLocation = Player->GetActorLocation();
    const FVector AiLocation     = Ai->GetPawn()->GetActorLocation();
    const FVector AiToPlayer     = FVector(PlayerLocation.X - AiLocation.X, PlayerLocation.Y - AiLocation.Y, 0);
    const float   DistFromPlayer = AiToPlayer.Size2D();
    Ai->CheckIfInPlayerRange(DistFromPlayer);

    // TODO.
    Ai->GetBlackboard()->SetValueAsBool("Attacking", false);

    // Fail task to end attack.
    return EBTNodeResult::Failed;
}
