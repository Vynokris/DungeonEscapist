


#include "TropheeActor.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#define EQUIPMENT_STENCIL_VAL 4

#pragma region Setup
ATropheeActor::ATropheeActor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* EquipmentRootComponent = CreateDefaultSubobject<USceneComponent>("TropheeRoot");
	RootComponent  = EquipmentRootComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("TropheeMesh");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetRenderCustomDepth(true);
	MeshComponent->SetCustomDepthStencilValue(EQUIPMENT_STENCIL_VAL);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("TropheeBox");
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);

	Tags.Add("Trophee");
}

bool ATropheeActor::WasPickedUp() const
{
	return ParentCharacter != nullptr;
}

void ATropheeActor::BeginPlay()
{
	Super::BeginPlay();
}
#pragma regionend


#pragma region Update
void ATropheeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + FVector (0,0, sin(UGameplayStatics::GetRealTimeSeconds(GetWorld())*2)/4));
	SetActorRotation(GetActorRotation() + FRotator(0, 1, 0));
}

void ATropheeActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}
#pragma regionend


#pragma region Getter & Setter
UStaticMeshComponent* ATropheeActor::GetMesh() const
{
	return IsValid(MeshComponent) ? MeshComponent : nullptr;
}
#pragma regionend

