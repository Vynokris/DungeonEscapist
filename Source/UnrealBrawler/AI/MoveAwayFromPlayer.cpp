#include "MoveAwayFromPlayer.h"

#include "DrawDebugHelpers.h"
#include "EnemyAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UMoveAwayFromPlayer::UMoveAwayFromPlayer()
{
    NodeName = "Move away from player";
}

static FVector ComputeStrafeVec(AEnemyAiController* Ai, const FVector& AiToPlayerDir)
{
    return (-AiToPlayerDir * 60.f + FVector::CrossProduct(AiToPlayerDir, FVector::UpVector) * Ai->GetStrafeDir() * 40.f) * 1.5f;
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
    const FVector AiToPlayer     = FVector(PlayerLocation.X - AiLocation.X, PlayerLocation.Y - AiLocation.Y, AiLocation.Z);
    const float   DistFromPlayer = AiToPlayer.Size2D();
    const FVector AiToPlayerDir  = AiToPlayer / DistFromPlayer;
    Ai->CheckIfInPlayerRange(DistFromPlayer);

    // Move away if the player is too close.
    if (Ai->IsInPlayerRange())
    {
        // Get the vector along which the AI should strafe.
        FVector StrafeVec = ComputeStrafeVec(Ai, AiToPlayerDir);

        // If there is an object in that direction, change the strafe direction and re-calculate the strafe vector.
        FHitResult HitStatic, HitDynamic;
        FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams(); CollisionQueryParams.AddIgnoredActor(Ai->GetPawn());
        if (GetWorld()->LineTraceSingleByChannel(HitStatic,  AiLocation, AiLocation + StrafeVec, ECC_WorldStatic,  CollisionQueryParams) ||
            GetWorld()->LineTraceSingleByChannel(HitDynamic, AiLocation, AiLocation + StrafeVec, ECC_WorldDynamic, CollisionQueryParams))
        {
            Ai->ChangeStrafeDir();
            StrafeVec = ComputeStrafeVec(Ai, AiToPlayerDir);
        }
    
        // Set the point towards which the AI should move.
        Ai->MoveToLocation(AiLocation + StrafeVec);
    }
    else
    {
        AiBlackboard->SetValueAsBool("TooCloseToPlayer", false);
    }

    // Finish with success.
    return EBTNodeResult::Succeeded;
}
