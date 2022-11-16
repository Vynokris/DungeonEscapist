#include "EnemyAiController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AEnemyAiController::AEnemyAiController(const FObjectInitializer& ObjectInitializer)
{
    // Get the behavior tree.
    static ConstructorHelpers::FObjectFinder<UBehaviorTree> Obj(TEXT("BehaviorTree'/Game/Blueprints/AI/BrawlerNpcBehavior.BrawlerNpcBehavior'"));
    if (Obj.Succeeded()) {
        BehaviorTree = Obj.Object;
    }

    // Create the behavior tree and blackboard components.
    BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
    Blackboard            = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent  >(this, TEXT("Blackboard"));
}

void AEnemyAiController::BeginPlay()
{
    Super::BeginPlay();

    // Start the behavior tree.
    if (BehaviorTree && BehaviorTreeComponent)
    {
        RunBehaviorTree(BehaviorTree);
        BehaviorTreeComponent->StartTree(*BehaviorTree);
    }

    // Get a random strafe direction.
    StrafeDir = FMath::RandRange(0, 1) == 0 ? 1 : -1;

    // Set the AI focus to the player.
    SetFocus(Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)), EAIFocusPriority::Gameplay);
}

void AEnemyAiController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Initialize the blackboard.
    if (Blackboard && BehaviorTree)
        Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
}

UBlackboardComponent* AEnemyAiController::GetBlackboard() const
{
    return Blackboard;
}

bool AEnemyAiController::IsInPlayerRange() const
{
    return InPlayerRange;
}

void AEnemyAiController::CheckIfInPlayerRange(const float DistFromPlayer)
{
    InPlayerRange = DistFromPlayer <= TargetDistFromPlayer;
}

int AEnemyAiController::GetStrafeDir() const
{
    return StrafeDir;
}

void AEnemyAiController::ChangeStrafeDir()
{
    StrafeDir *= -1;
}

float AEnemyAiController::GetTargetDistFromPlayer() const
{
    return TargetDistFromPlayer;
}

float AEnemyAiController::GetPlayerRangeSize() const
{
    return PlayerRangeSize;
}
