


#include "KnifeActor.h"

#include "DebugUtils.h"
#include "Components/BoxComponent.h"

AKnifeActor::AKnifeActor()
{
	PrimaryActorTick.bCanEverTick = true;

	KnifeRoot = CreateDefaultSubobject<USceneComponent>(TEXT("KnifeRoot"));
	RootComponent = KnifeRoot;

	KnifeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KnifeMesh"));
	KnifeMesh->AttachToComponent(KnifeRoot, FAttachmentTransformRules::SnapToTargetIncludingScale);

	KnifeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Knifebox"));
	KnifeBox->SetWorldScale3D(FVector(0.3f,0.3f,1.0f));
	KnifeBox->SetGenerateOverlapEvents(true);
	KnifeBox->OnComponentBeginOverlap.AddDynamic(this, &AKnifeActor::OnHit);
	KnifeBox->AttachToComponent(KnifeRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AKnifeActor::BeginPlay()
{
	DebugWarning("Spawning AKnifeActor at = %s", *this->GetActorLocation().ToString());
	
	Super::BeginPlay();
}

void AKnifeActor::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
	
	this->GetActorLocation().Set(0, this->GetActorLocation().Y + sin(_DeltaTime), 0);
}


