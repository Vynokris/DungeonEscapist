#include "MoveAwayFromPlayer.h"

#include "EnemyAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UMoveAwayFromPlayer::UMoveAwayFromPlayer()
{
    NodeName = "Move away from player";
}

EBTNodeResult::Type UMoveAwayFromPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Get the player and AI controller.
    if (!Player) {
        Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    }
    AEnemyAiController*   Ai           = Cast<AEnemyAiController>(OwnerComp.GetAIOwner());
    UBlackboardComponent* AiBlackboard = Ai->GetBlackboard();

    // Get the player and AI locations as well as their distance.
    const FVector PlayerLocation = Player->GetActorLocation();
    const FVector AiLocation     = Ai->GetPawn()->GetActorLocation();
    const FVector AiToPlayer     = FVector(PlayerLocation.X - AiLocation.X, PlayerLocation.Y - AiLocation.Y, 0);
    const float   DistFromPlayer = AiToPlayer.Size2D();
    Ai->CheckIfInPlayerRange(DistFromPlayer);

    // Move away if the player is too close.
    if (Ai->IsInPlayerRange()) {
        Ai->MoveToLocation(AiLocation - AiToPlayer);
    }
    else {
        AiBlackboard->SetValueAsBool("TooCloseToPlayer", false);
    }

    // Finish with success.
    return EBTNodeResult::Succeeded;
}
