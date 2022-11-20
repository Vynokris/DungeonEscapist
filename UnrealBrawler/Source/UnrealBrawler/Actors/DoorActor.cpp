


#include "DoorActor.h"

#include "../BrawlerCharacter.h"
#include "../Utils/DebugUtils.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoorActor::ADoorActor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* DoorRootComponent = CreateDefaultSubobject<USceneComponent>("DoorRoot");
	RootComponent  = DoorRootComponent;


	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("DoorMesh");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	MeshComponent->SetGenerateOverlapEvents(true);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("DoorBox");
	BoxComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	BoxComponent->SetGenerateOverlapEvents(true);

	BlockBoxComponent = CreateDefaultSubobject<UBoxComponent>("BlockBox");
	BlockBoxComponent->SetupAttachment(RootComponent);
	BlockBoxComponent->SetGenerateOverlapEvents(false);
	
	Tags.Add("Door");
}

void ADoorActor::BeginPlay()
{
	//Debug("Spawning DoorActor at = %s", *this->GetActorLocation().ToString());
	Super::BeginPlay();
}

void ADoorActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(IsClosing() && !IsClosed())
	{
		MeshComponent->AddRelativeLocation(FVector (0,0, endZLocation/ClosingRate * DeltaSeconds));
		if(MeshComponent->GetRelativeLocation().Z >= endZLocation)
		{
			DoorClosed = true;
			DoorClosing = false;
		}
	}
}

void ADoorActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if(!OtherActor->IsA(ABrawlerCharacter::StaticClass())) return;
	
	const ABrawlerCharacter* BrawlerCharacter = Cast<ABrawlerCharacter>(OtherActor);
	if(IsValid(BrawlerCharacter) && BrawlerCharacter->IsPlayer() && !IsClosing() && !IsClosed()) CloseDoorEvent();
}

void ADoorActor::OpenDoorEvent()
{
	DoorClosing = false;
}

void ADoorActor::CloseDoorEvent()
{
	DoorClosing = true;
	BlockBoxComponent->SetCollisionResponseToAllChannels(ECR_Block);
}

bool ADoorActor::IsClosing() const
{
	return DoorClosing;
}

bool ADoorActor::IsClosed() const
{
	return DoorClosed;
}




