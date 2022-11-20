#include "EquipmentActor.h"

#include "../BrawlerCharacter.h"
#include "../Utils/DebugUtils.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#define EQUIPMENT_STENCIL_VAL 4

AEquipmentActor::AEquipmentActor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* EquipmentRootComponent = CreateDefaultSubobject<USceneComponent>("EquipmentRoot");
	RootComponent  = EquipmentRootComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("EquipmentMesh");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    MeshComponent->SetRenderCustomDepth(true);
	MeshComponent->SetCustomDepthStencilValue(EQUIPMENT_STENCIL_VAL);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("EquipmentBox");
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxComponent->SetGenerateOverlapEvents(true);

	Tags.Add("Equipment");
}

void AEquipmentActor::BeginPlay()
{
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxComponent->SetGenerateOverlapEvents(true);
	
	switch (EquipmentType)
	{
	case Weapon:
		Tags.Add("Weapon");
		break;
	case Shield:
		Tags.Add("Shield");
		break;
	default:
		break;
	}
	
	Super::BeginPlay();
}

void AEquipmentActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (PlayFloatingAnimation && !WasPickedUp()) {
		SetActorLocation(GetActorLocation() + FVector (0,0, sin(UGameplayStatics::GetRealTimeSeconds(GetWorld())*2)/4));
		SetActorRotation(GetActorRotation() + FRotator(0, 1, 0));
	}
}

void AEquipmentActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// Get picked up only by the player.
	if (!WasPickedUp()) {
		if (OtherActor->Tags.Contains("Player")) {
			GetPickedUp(Cast<ABrawlerCharacter>(OtherActor));
		}
	}

	// If the equipment is held, let it perform actions only if its parent character isn't dead and isn't the one the equipment collided with.
	else if (!ParentCharacter->IsDead() && Cast<AActor>(ParentCharacter) != OtherActor)
	{
		switch (EquipmentType)
		{
		case Weapon:
			// Let player and enemies damage each other with a weapon.
			if (ParentCharacter->IsAttacking() && !ParentCharacter->WasAttackBlocked())
			{
				if (ABrawlerCharacter* OtherCharacter = Cast<ABrawlerCharacter>(OtherActor))
				{
					if ((ParentCharacter->IsEnemy()  &&  OtherCharacter->IsPlayer()) ||
						(ParentCharacter->IsPlayer() && !OtherCharacter->IsPlayer()))
					{
						OtherCharacter->TakeDamageEvent(ParentCharacter->GetAttackDamage());
					}
				}
			}
			break;
			
		case Shield:
			// If the shield was hit by a weapon, block the other character's attack.
			if (ParentCharacter->IsDefending() && OtherActor->Tags.Contains("Weapon"))
				if (ABrawlerCharacter* OtherCharacter = Cast<AEquipmentActor>(OtherActor)->ParentCharacter)
					OtherCharacter->AttackBlockedEvent();
			break;
			
		default:
			break;
		}
	}
}

void AEquipmentActor::GetPickedUp(ABrawlerCharacter* NewParentCharacter)
{
	if (!NewParentCharacter) return;
	
	FName SocketName = "";
	switch (EquipmentType)
	{
	case Weapon:
		if (NewParentCharacter->HasEquipment(Weapon)) return;
		SocketName = "WeaponSocket";
		break;
	case Shield:
		if (NewParentCharacter->HasEquipment(Shield)) return;
		SocketName = "ShieldSocket";
		break;
	default:
		break;
	}
	
	AttachToComponent(Cast<USceneComponent>(NewParentCharacter->GetMesh()),
					  FAttachmentTransformRules::SnapToTargetNotIncludingScale,
					  SocketName);
	SetActorRelativeLocation({0, 0, 0});
	SetActorRelativeRotation({0, 0, 0});
	NewParentCharacter->PickupEquipmentEvent(this);
	MeshComponent->SetCustomDepthStencilValue(NewParentCharacter->GetMesh()->CustomDepthStencilValue);
	ParentCharacter = NewParentCharacter;
}

void AEquipmentActor::GetDropped(const ABrawlerCharacter* CurParentCharacter)
{
	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	SetActorRelativeLocation(CurParentCharacter->GetActorLocation());
	SetActorRelativeRotation({0, 0, 0});
	SetActorScale3D({1,1,1});
	MeshComponent->SetCustomDepthStencilValue(EQUIPMENT_STENCIL_VAL);
	ParentCharacter = nullptr;
	PlayFloatingAnimation = true;
}

void AEquipmentActor::ParentDeathEvent() const
{
	GetMesh()->SetRenderCustomDepth(false);
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
}

bool AEquipmentActor::WasPickedUp() const
{
	return ParentCharacter != nullptr;
}

EEquipmentType AEquipmentActor::GetType() const
{
	return EquipmentType;
}

UStaticMeshComponent* AEquipmentActor::GetMesh() const
{
	return MeshComponent;
}
