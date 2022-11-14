#include "MoveToPlayer.h"

#include "EnemyAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UMoveToPlayer::UMoveToPlayer()
{
    NodeName = "Move to player";
}

EBTNodeResult::Type UMoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Note: This function's return values look inverted because this node is used in a selector before the AttackPlayer node.
    //       This means that this node needs to return Success to prevent the execution of the AttackPlayer node.
    //       When this node finally returns Failed, the selector calls the AttackPlayer node.
    
    // Get the player and AI controller.
    if (!Player) {
        Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    }
    AEnemyAiController* Ai = Cast<AEnemyAiController>(OwnerComp.GetAIOwner());

    // Get the player and AI locations as well as their distance.
    const FVector PlayerLocation = Player->GetActorLocation();
    const FVector AiLocation     = Ai->GetPawn()->GetActorLocation();
    const FVector AiToPlayer     = FVector(PlayerLocation.X - AiLocation.X, PlayerLocation.Y - AiLocation.Y, 0);
    const float   DistFromPlayer = AiToPlayer.Size2D();
    Ai->CheckIfInPlayerRange(DistFromPlayer);

    // Stop attacking if the player is out of range.
    if (!Ai->IsInPlayerRange())
    {
        Ai->GetBlackboard()->SetValueAsBool("Attacking", false);
        return EBTNodeResult::Succeeded;
    }
    
    // Move to the player location.
    if (DistFromPlayer > 120.f)
    {
        Ai->MoveToLocation(Player->GetActorLocation());
        return EBTNodeResult::Succeeded;
    }
    Ai->StopMovement();

    // Finish with success.
    return EBTNodeResult::Failed;
}
