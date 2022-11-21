#include "HeartActor.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealBrawler/BrawlerCharacter.h"

#define EQUIPMENT_STENCIL_VAL 4

#pragma region Setup
AHeartActor::AHeartActor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* EquipmentRootComponent = CreateDefaultSubobject<USceneComponent>("HeartRoot");
	RootComponent  = EquipmentRootComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("HeartMesh");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetRenderCustomDepth(true);
	MeshComponent->SetCustomDepthStencilValue(EQUIPMENT_STENCIL_VAL);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("HeartBox");
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxComponent->SetGenerateOverlapEvents(true);

	Tags.Add("Heart");
}

void AHeartActor::BeginPlay()
{
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxComponent->SetGenerateOverlapEvents(true);
	
	Super::BeginPlay();
}
#pragma endregion


#pragma region Update
void AHeartActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + FVector (0,0, sin(UGameplayStatics::GetRealTimeSeconds(GetWorld())*2)/4));
	SetActorRotation(GetActorRotation() + FRotator(0, 1, 0));
}

void AHeartActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->Tags.Contains("Player"))
	{
		ABrawlerCharacter* Player = Cast<ABrawlerCharacter>(OtherActor);
		if (IsValid(Player))
			Player->HealEvent(FMath::RandRange(1, 2));
		Destroy();
	}
}
#pragma endregion


#pragma region Getter & Setter
UStaticMeshComponent* AHeartActor::GetMesh() const
{
	return IsValid(MeshComponent) ? MeshComponent : nullptr;
}
#pragma endregion

