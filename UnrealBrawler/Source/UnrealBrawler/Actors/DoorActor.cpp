#include "DoorActor.h"
#include "../BrawlerCharacter.h"
#include "Engine/TriggerBox.h"
#include "Kismet/GameplayStatics.h"

#define EQUIPMENT_STENCIL_VAL 4

// Sets default values
ADoorActor::ADoorActor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* DoorRootComponent = CreateDefaultSubobject<USceneComponent>("DoorRoot");
	RootComponent  = DoorRootComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("DoorMesh");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRenderCustomDepth(false);
	MeshComponent->SetCustomDepthStencilValue(EQUIPMENT_STENCIL_VAL);

	Tags.Add("Door");
}

void ADoorActor::BeginPlay()
{
	Super::BeginPlay();

	// Setup the trigger callback.
	if (EntranceTrigger)
		EntranceTrigger->OnActorBeginOverlap.AddDynamic(this, &ADoorActor::OnTrigger);

	// Get the door mesh size.
	FVector BoundingBoxLocation, BoundingBoxExtent;
	float BoundingSphereRadius;
	UKismetSystemLibrary::GetComponentBounds(MeshComponent, BoundingBoxLocation, BoundingBoxExtent, BoundingSphereRadius);

	// Setup the door closed and open positions.
	if (IsClosed())
	{
		ZLocationClosed = GetActorLocation().Z;
		ZLocationOpen   = ZLocationClosed - BoundingBoxExtent.Z * 2;
	}
	else
	{
		ZLocationOpen   = GetActorLocation().Z;
		ZLocationClosed = ZLocationOpen + BoundingBoxExtent.Z * 2;
	}
}

void ADoorActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsMoving())
	{
		AddActorWorldOffset({ 0,0, ClosingRate * (IsClosed() ? -1 : 1) * DeltaSeconds });
		
		if (IsClosed())
		{
			if (GetActorLocation().Z <= ZLocationOpen)
			{
				DoorClosed = false;
				DoorMoving = false;
				SetActorLocation({ GetActorLocation().X, GetActorLocation().Y, ZLocationOpen });
				MeshComponent->SetRenderCustomDepth(false);
			}
		}
		else
		{
			if (GetActorLocation().Z >= ZLocationClosed)
			{
				DoorClosed = true;
				DoorMoving = false;
				SetActorLocation({ GetActorLocation().X, GetActorLocation().Y, ZLocationClosed });
				MeshComponent->SetRenderCustomDepth(false);
			}
		}
	}
}

void ADoorActor::OnTrigger(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->Tags.Contains("Player") && !IsMoving()) {
		MoveDoorEvent();
		if (TriggerOnce) {
            EntranceTrigger->OnActorBeginOverlap.RemoveDynamic(this, &ADoorActor::OnTrigger);
		}
	}
}

void ADoorActor::MoveDoorEvent()
{
	DoorMoving = true;
	MeshComponent->SetRenderCustomDepth(true);
}

bool ADoorActor::IsMoving() const
{
	return DoorMoving;
}

bool ADoorActor::IsClosed() const
{
	return DoorClosed;
}




