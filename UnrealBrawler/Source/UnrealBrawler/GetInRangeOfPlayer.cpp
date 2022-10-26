#include "GetInRangeOfPlayer.h"

#include "BrawlerNpc.h"
#include "BrawlerNpcAi.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DebugUtils.h"

UGetInRangeOfPlayer::UGetInRangeOfPlayer(const FObjectInitializer& ObjectInitializer)
{
    NodeName = TEXT("Get In Range Of Player");
}

EBTNodeResult::Type UGetInRangeOfPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Get the player and AI controller.
    if (!Player)
        Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    ABrawlerNpcAi* Ai = Cast<ABrawlerNpcAi>(OwnerComp.GetAIOwner());

    // Get the player and AI locations as well as their distance.
    const FVector PlayerLocation = Player->GetActorLocation();
    const FVector AiLocation     = Ai->GetPawn()->GetActorLocation();
    const FVector AiToPlayer     = FVector(PlayerLocation.X - AiLocation.X, PlayerLocation.Y - AiLocation.Y, 0);
    const float   DistFromPlayer = AiToPlayer.Size2D();

    // If the AI doesn't have line of sight to the player, make it move laterally.
    FHitResult HitStatic, HitDynamic;
    FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams(); CollisionQueryParams.AddIgnoredActor(Ai->GetPawn()); CollisionQueryParams.AddIgnoredActor(Player);
    if (GetWorld()->LineTraceSingleByChannel(HitStatic,  AiLocation, PlayerLocation, ECollisionChannel::ECC_WorldStatic,  CollisionQueryParams) ||
        GetWorld()->LineTraceSingleByChannel(HitDynamic, AiLocation, PlayerLocation, ECollisionChannel::ECC_WorldDynamic, CollisionQueryParams))
    {
        const FVector AiToPlayerDir = AiToPlayer / AiToPlayer.Size();
        Ai->MoveToLocation(AiLocation + AiToPlayerDir * 50.f + FVector::CrossProduct(AiToPlayerDir * 100.f, FVector::UpVector) * ((ABrawlerNpc*)Ai->GetPawn())->GetStrafeDir());

        // Finish with success.
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return EBTNodeResult::Succeeded;
    }

    // Move towards the player if the AI isn't close enough.
    if (DistFromPlayer > TargetDistFromPlayer)
        Ai->MoveToLocation(Player->GetActorLocation());

    // Move away from the player if the AI is too close.
    else if (DistFromPlayer + RangeSize < TargetDistFromPlayer)
        Ai->MoveToLocation(AiLocation - AiToPlayer);

    // Don't move when in range.
    else
        Ai->StopMovement();

    // Finish with success.
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}

