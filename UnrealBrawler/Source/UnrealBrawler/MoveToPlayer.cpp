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

    // Move to the player location.
    if (DistFromPlayer > 80.f)
    {
        Ai->MoveToLocation(Player->GetActorLocation());
        return EBTNodeResult::Succeeded;
    }

    // Finish with success.
    return EBTNodeResult::Failed;
}
