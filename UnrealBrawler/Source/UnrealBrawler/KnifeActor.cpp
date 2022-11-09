#include "KnifeActor.h"

#include "BrawlerCharacter.h"
#include "DebugUtils.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AKnifeActor::AKnifeActor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* KnifeRootComponent = CreateDefaultSubobject<USceneComponent>("KnifeRoot");
	RootComponent = KnifeRootComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("KnifeMesh");
	// MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetStaticMesh(KnifeMesh);
	MeshComponent->SetRelativeScale3D({0.005, 0.005, 0.005});

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("KnifeBox");
	BoxComponent->SetWorldScale3D(FVector(0.3f,0.3f,1.0f));
	BoxComponent->SetGenerateOverlapEvents(true);
	// BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AKnifeActor::OnHit);
	// BoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	BoxComponent->SetupAttachment(RootComponent);
}

void AKnifeActor::BeginPlay()
{
	Debug("Spawning AKnifeActor at = %s", *this->GetActorLocation().ToString());
	MeshComponent->SetStaticMesh(KnifeMesh);
	
	Super::BeginPlay();
}

void AKnifeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!WasPickedUp()) {
		SetActorLocation(GetActorLocation() + FVector (0,0, sin(UGameplayStatics::GetRealTimeSeconds(GetWorld())*2)/4));
		SetActorRotation(GetActorRotation() + FRotator(0, 1, 0));
	}
}

void AKnifeActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (!WasPickedUp() && OtherActor->Tags.Contains("Player"))
	{
		AttachToComponent(Cast<USceneComponent>(Cast<ABrawlerCharacter>(OtherActor)->GetMesh()),
						  FAttachmentTransformRules::SnapToTargetNotIncludingScale,
						  "WeaponSocket");
		SetActorRelativeLocation({0, 0, 0});
		SetActorRelativeRotation({0, 0, 0});
		Player = Cast<ABrawlerCharacter>(OtherActor);
	}

	if (WasPickedUp() && Player->IsAttacking() && OtherActor->Tags.Contains("Enemy"))
	{
		Debug("Touched an enemy");
	}
}

bool AKnifeActor::WasPickedUp() const
{
	return Player != nullptr;
}
