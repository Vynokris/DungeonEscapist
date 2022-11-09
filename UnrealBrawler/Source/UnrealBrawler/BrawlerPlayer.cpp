#include "BrawlerPlayer.h"

#include "BrawlerNpc.h"
#include "BrawlerNpcAi.h"
#include "DebugUtils.h"
#include "KnifeActor.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"


ABrawlerPlayer::ABrawlerPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	// Setup camera spring arm.
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	SpringArmComponent->bEnableCameraLag        = true;
	SpringArmComponent->CameraLagSpeed          = CameraLag;
	SpringArmComponent->TargetArmLength         = CameraDistance;
	SpringArmComponent->bUsePawnControlRotation = true;

	// Setup camera.
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = true;

	// Setup character movement.
	CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->bUseControllerDesiredRotation = false;
	CharacterMovementComponent->bOrientRotationToMovement     = true;

	// Setup walking particles.
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
}

void ABrawlerPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Update camera parameters.
	SpringArmComponent->CameraLagSpeed = CameraLag;
	SpringArmComponent->TargetArmLength = CameraDistance;

	// If the character is controller by AI, add the Enemy tag and make the character always face the player.
	if (ABrawlerNpcAi* Ai = Cast<ABrawlerNpcAi>(GetController()))
	{
		Tags.Add("Enemy");
		CharacterMovementComponent->bUseControllerDesiredRotation = true;
		CharacterMovementComponent->bOrientRotationToMovement     = false;
		Ai->SetFocus(Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)), EAIFocusPriority::Gameplay);
	}

	// If the character is controlled by the player, add the Player tag.
	else
	{
		Tags.Add("Player");
	}
}

void ABrawlerPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateWalkingFX();

	// Decrement timers.
	if (InvincibilityTimer > 0)
		InvincibilityTimer -= DeltaSeconds;
	if (AttackTimer > 0)
		AttackTimer -= DeltaSeconds;
}

void ABrawlerPlayer::SetupPlayerInputComponent(UInputComponent* NewInputComponent)
{
	Super::SetupPlayerInputComponent(NewInputComponent);

	NewInputComponent->BindAxis("MoveForward", this, &ABrawlerPlayer::MoveForward);
	NewInputComponent->BindAxis("MoveRight", this, &ABrawlerPlayer::MoveRight);

	NewInputComponent->BindAxis("LookRight", this, &ABrawlerPlayer::AddControllerYawInput);
	NewInputComponent->BindAxis("LookUp", this, &ABrawlerPlayer::AddControllerPitchInput);

	// _inputComponent->BindAction("TakeDamage", IE_Pressed, this, &ABrawlerPlayer::TakeDamage);
	{
		FInputActionBinding TakeDamageBinding("TestKey", IE_Pressed);
		TakeDamageBinding.ActionDelegate.GetDelegateForManualSet().BindLambda([this] { TakeDamageEvent(1); });
		NewInputComponent->AddActionBinding(TakeDamageBinding);
	}
}


void ABrawlerPlayer::MoveForward(const float Amount)
{
	if(IsDead()) return;

	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X)
					        + FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Z);
	AddMovementInput(Direction, Amount);
}

void ABrawlerPlayer::MoveRight(const float Amount)
{
	if(IsDead()) return;

	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Amount);
}

void ABrawlerPlayer::UpdateWalkingFX() const
{
	if (IsDead()) return;
	
	const bool ShouldShowFX = GetVelocity() != FVector(0, 0, 0) && !CharacterMovementComponent->IsFalling();
	
	if (ShouldShowFX && ParticleSystemComponent->bWasDeactivated)
		ParticleSystemComponent->ActivateSystem();
	else if (!ShouldShowFX && !ParticleSystemComponent->bWasDeactivated)
		ParticleSystemComponent->DeactivateSystem();
}


void ABrawlerPlayer::TakeDamageEvent(const int& Amount)
{
	if (InvincibilityTimer > 0) {
		Debug("Player is invincible.");
		return;
	}
	
	Health -= Amount;
	if (Health <= 0) {
		DeathEvent();
		Debug("Player is dead!");
	}
	else {
		Debug("Player hit, lost: %d HP", Amount);
	}
}

void ABrawlerPlayer::AttackEvent()
{
	AttackTimer = AttackDuration;
}

void ABrawlerPlayer::StartDefendingEvent()
{
	Defending = true;
}

void ABrawlerPlayer::StopDefendingEvent()
{
	Defending = false;
}

void ABrawlerPlayer::InvincibilityEvent()
{
	InvincibilityTimer = InvincibilityDuration;
}

void ABrawlerPlayer::DeathEvent()
{
	ParticleSystemComponent->DeactivateSystem();
	Health = 0;
}

void ABrawlerPlayer::EnemyKilledEvent()
{
	KillCount++;
	GameHudComponent->UpdateEnemyCounterEvent(KillCount);
}


int ABrawlerPlayer::GetHealth() const
{
	return Health;
}

int ABrawlerPlayer::GetStamina() const
{
	return Stamina;
}

int ABrawlerPlayer::GetKillCount() const
{
	return KillCount;
}

bool ABrawlerPlayer::IsAttacking() const
{
	return AttackTimer > 0;
}

bool ABrawlerPlayer::IsDefending() const
{
	return Defending;
}

bool ABrawlerPlayer::IsInvincible() const
{
	return InvincibilityTimer > 0;
}

bool ABrawlerPlayer::IsDead() const
{
	return Health <= 0;
}
