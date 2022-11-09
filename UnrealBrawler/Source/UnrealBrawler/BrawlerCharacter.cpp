#include "BrawlerCharacter.h"

#include "EnemyAiController.h"
#include "DebugUtils.h"
#include "KnifeActor.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"


ABrawlerCharacter::ABrawlerCharacter()
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

void ABrawlerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Update camera parameters.
	SpringArmComponent->CameraLagSpeed = CameraLag;
	SpringArmComponent->TargetArmLength = CameraDistance;

	// If the character is controller by AI, treat this character as an enemy.
	if (AEnemyAiController* Ai = Cast<AEnemyAiController>(GetController()))
	{
		CharacterIsPlayer = false;
		Health = EnemyMaxHealth;
		Tags.Add("Enemy");

		// Force the enemy to always face the player.
		CharacterMovementComponent->bUseControllerDesiredRotation = true;
		CharacterMovementComponent->bOrientRotationToMovement     = false;
		Ai->SetFocus(Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)), EAIFocusPriority::Gameplay);

		// Remove the camera and spring arm.
		GetComponentByClass(UCameraComponent   ::StaticClass())->DestroyComponent();
		GetComponentByClass(USpringArmComponent::StaticClass())->DestroyComponent();

		// Change the movement speed.
		GetCharacterMovement()->MaxWalkSpeed = 500;

		// Give a knife to the enemy.
		if (EnemyDefaultWeapon)
		{
			AKnifeActor* Knife = Cast<AKnifeActor>(GetWorld()->SpawnActor(EnemyDefaultWeapon));
			Knife->GetPickedUp(this);
		}
	}

	// If the character is controlled by the player, treat this character as a player.
	else
	{
		Health = PlayerMaxHealth;
		Tags.Add("Player");

		// Give a knife to the player.
		if (PlayerDefaultWeapon)
		{
			AKnifeActor* Knife = Cast<AKnifeActor>(GetWorld()->SpawnActor(PlayerDefaultWeapon));
			Knife->GetPickedUp(this);
		}
	}
}

void ABrawlerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateWalkingFX();

	// Decrement timers.
	if (InvincibilityTimer > 0)
		InvincibilityTimer -= DeltaSeconds;
	if (AttackTimer > 0)
		AttackTimer -= DeltaSeconds;
}

void ABrawlerCharacter::SetupPlayerInputComponent(UInputComponent* NewInputComponent)
{
	Super::SetupPlayerInputComponent(NewInputComponent);

	// Movement.
	NewInputComponent->BindAxis("MoveForward", this, &ABrawlerCharacter::MoveForward);
	NewInputComponent->BindAxis("MoveRight", this, &ABrawlerCharacter::MoveRight);

	// Camera.
	NewInputComponent->BindAxis("LookRight", this, &ABrawlerCharacter::AddControllerYawInput);
	NewInputComponent->BindAxis("LookUp", this, &ABrawlerCharacter::AddControllerPitchInput);

	// Attack key.
	NewInputComponent->BindAction("Attack", IE_Pressed, this, &ABrawlerCharacter::AttackEvent);
	
	// Defend key.
	NewInputComponent->BindAction("Defend", IE_Pressed,  this, &ABrawlerCharacter::StartDefendingEvent);
	NewInputComponent->BindAction("Defend", IE_Released, this, &ABrawlerCharacter::StopDefendingEvent);
	
	// Damage test key.
	// NewInputComponent->BindAction("TakeDamage", IE_Pressed, this, &ABrawlerCharacter::TakeDamage);
	{
		FInputActionBinding TakeDamageBinding("TestKey", IE_Pressed);
		TakeDamageBinding.ActionDelegate.GetDelegateForManualSet().BindLambda([this] { TakeDamageEvent(1); });
		NewInputComponent->AddActionBinding(TakeDamageBinding);
	}
}


void ABrawlerCharacter::MoveForward(const float Amount)
{
	if(IsDead()) return;

	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X)
							+ FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Z);
	AddMovementInput(Direction, Amount);
}

void ABrawlerCharacter::MoveRight(const float Amount)
{
	if(IsDead()) return;

	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Amount);
}

void ABrawlerCharacter::UpdateWalkingFX() const
{
	if (IsDead()) return;
	
	const bool ShouldShowFX = GetVelocity() != FVector(0, 0, 0) && !CharacterMovementComponent->IsFalling();
	
	if (ShouldShowFX && ParticleSystemComponent->bWasDeactivated)
		ParticleSystemComponent->ActivateSystem();
	else if (!ShouldShowFX && !ParticleSystemComponent->bWasDeactivated)
		ParticleSystemComponent->DeactivateSystem();
}


void ABrawlerCharacter::TakeDamageEvent(const int& Amount)
{
	if (InvincibilityTimer > 0) {
		return;
	}
	
	Health -= Amount;
	if (Health <= 0) {
		DeathEvent();
		Debug("%s is dead!", *GetName());
	}
	else {
		InvincibilityEvent();
		Debug("%s hit, lost %d HP, %d HP remaining", *GetName(), Amount, Health);
	}
}

void ABrawlerCharacter::AttackEvent()
{
	AttackTimer = AttackDuration;
}

void ABrawlerCharacter::StartDefendingEvent()
{
	Defending = true;
}

void ABrawlerCharacter::StopDefendingEvent()
{
	Defending = false;
}

void ABrawlerCharacter::InvincibilityEvent()
{
	InvincibilityTimer = InvincibilityDuration;
}

void ABrawlerCharacter::DeathEvent()
{
	Health = 0;
	ParticleSystemComponent->DeactivateSystem();
	if (Controller && IsEnemy())
		Controller->UnPossess();
}

void ABrawlerCharacter::EnemyKilledEvent()
{
	KillCount++;
	GameHudComponent->UpdateEnemyCounterEvent(KillCount);
}


int ABrawlerCharacter::IsPlayer() const
{
	return CharacterIsPlayer;
}

int ABrawlerCharacter::IsEnemy() const
{
	return !CharacterIsPlayer;
}

int ABrawlerCharacter::GetHealth() const
{
	return Health;
}

int ABrawlerCharacter::GetAttackDamage() const
{
	return AttackDamage;
}

int ABrawlerCharacter::GetKillCount() const
{
	return KillCount;
}

bool ABrawlerCharacter::IsAttacking() const
{
	return AttackTimer > 0;
}

bool ABrawlerCharacter::IsDefending() const
{
	return Defending;
}

bool ABrawlerCharacter::IsInvincible() const
{
	return InvincibilityTimer > 0;
}

bool ABrawlerCharacter::IsDead() const
{
	return Health <= 0;
}
