#include "MoveIntoPlayerRange.h"

#include "BrawlerNpcAi.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UMoveIntoPlayerRange::UMoveIntoPlayerRange(const FObjectInitializer& ObjectInitializer)
{
    NodeName = TEXT("Move into player range");
}

EBTNodeResult::Type UMoveIntoPlayerRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    // If the AI doesn't have line of sight to the player, make it strafe.
    FHitResult HitStatic, HitDynamic;
    FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams(); CollisionQueryParams.AddIgnoredActor(Ai->GetPawn()); CollisionQueryParams.AddIgnoredActor(Player);
    if (GetWorld()->LineTraceSingleByChannel(HitStatic,  AiLocation, PlayerLocation, ECC_WorldStatic,  CollisionQueryParams) ||
        GetWorld()->LineTraceSingleByChannel(HitDynamic, AiLocation, PlayerLocation, ECC_WorldDynamic, CollisionQueryParams))
    {
        const FVector AiToPlayerVec = AiToPlayer / DistFromPlayer;
        Ai->MoveToLocation(AiLocation + AiToPlayerVec * 50.f + FVector::CrossProduct(AiToPlayerVec * 100.f, FVector::UpVector) * Ai->GetStrafeDir());
        Ai->GetPawn()->SetActorRotation(AiToPlayer.Rotation());
        return EBTNodeResult::Failed;
    }

    // Move towards the player if the AI isn't close enough.
    if (!Ai->IsInPlayerRange())
    {
        Ai->MoveToLocation(Player->GetActorLocation());
        return EBTNodeResult::Failed;
    }

    // Start moving away from the player if the AI is too close.
    else if (DistFromPlayer + Ai->GetPlayerRangeSize() < Ai->GetTargetDistFromPlayer())
    {
        AiBlackboard->SetValueAsBool("TooCloseToPlayer", true);
        return EBTNodeResult::Failed;
    }

    // Return success when the AI is in the player range.
    return EBTNodeResult::Succeeded;
}

