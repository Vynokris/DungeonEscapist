#include "StayInPlayerRange.h"

#include "BrawlerNpc.h"
#include "BrawlerNpcAi.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DebugUtils.h"
#include "DrawDebugHelpers.h"

UStayInPlayerRange::UStayInPlayerRange()
{
    NodeName = TEXT("Stay in player range");
}

FVector ComputeStrafeVec(ABrawlerNpcAi* Ai, const FVector& AiToPlayerDir)
{
    return (AiToPlayerDir * 20.f + FVector::CrossProduct(AiToPlayerDir * 100.f, FVector::UpVector) * Ai->GetStrafeDir()) * 1.2f;
}

EBTNodeResult::Type UStayInPlayerRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
    const FVector AiToPlayerDir  = AiToPlayer / AiToPlayer.Size2D();

    // Randomly change direction.
    if (FMath::RandRange(0, 50) == 0)
        Ai->ChangeStrafeDir();

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
    Ai->MoveTo(AiLocation + StrafeVec);

    // Finish with success.
    return EBTNodeResult::Succeeded;
}
