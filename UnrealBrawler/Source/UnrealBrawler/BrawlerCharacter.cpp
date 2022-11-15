#include "BrawlerCharacter.h"

#include "EnemyAiController.h"
#include "DebugUtils.h"
#include "EquipmentActor.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/WidgetTree.h"
#include "NiagaraFunctionLibrary.h"
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

        // Give default equipment to the enemy.
        for (TSubclassOf<AEquipmentActor> EquipmentPiece : EnemyDefaultEquipment)
            if (EquipmentPiece)
                Cast<AEquipmentActor>(GetWorld()->SpawnActor(EquipmentPiece))->GetPickedUp(this);
    }

    // If the character is controlled by the player, treat this character as a player.
    else
    {
        Health = PlayerMaxHealth;
        SetActorLabel("Player");
        Tags.Add("Player");

        // Give default equipment to the player.
        for (TSubclassOf<AEquipmentActor> EquipmentPiece : PlayerDefaultEquipment)
            if (EquipmentPiece)
                Cast<AEquipmentActor>(GetWorld()->SpawnActor(EquipmentPiece))->GetPickedUp(this);
    }
}

void ABrawlerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdateWalkingFX();

    // Attack/defend whenever possible if buffers are non-zero.
    if (DefenseBuffer > 0 && !IsAttacking() && !IsDefending()) StartDefendingEvent();
    if (AttackBuffer  > 0 && !IsAttacking() && !IsDefending()) StartAttackingEvent();

    // Decrement timer and buffers.
    if (InvincibilityTimer > 0) InvincibilityTimer -= DeltaSeconds;
    if (AttackBuffer       > 0) AttackBuffer       -= DeltaSeconds;
    if (DefenseBuffer      > 0) DefenseBuffer      -= DeltaSeconds;
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
	NewInputComponent->BindAction("Attack", IE_Pressed, this, &ABrawlerCharacter::StartAttackingEvent);
	
	// Defend key.
	NewInputComponent->BindAction("Defend", IE_Pressed,  this, &ABrawlerCharacter::StartDefendingEvent);
	NewInputComponent->BindAction("Defend", IE_Released, this, &ABrawlerCharacter::StopDefendingEvent);
	
	// Drop keys.
	{
		FInputActionBinding DropWeaponBinding("DropWeapon", IE_Pressed);
		DropWeaponBinding.ActionDelegate.GetDelegateForManualSet().BindLambda([this] { DropEquipmentEvent(Weapon); });
		NewInputComponent->AddActionBinding(DropWeaponBinding);
		
		FInputActionBinding DropShieldBinding("DropShield", IE_Pressed);
		DropShieldBinding.ActionDelegate.GetDelegateForManualSet().BindLambda([this] { DropEquipmentEvent(Shield); });
		NewInputComponent->AddActionBinding(DropShieldBinding);
	}

    // Enemy counter test key;
    NewInputComponent->BindAction("TestKey", IE_Pressed, this, &ABrawlerCharacter::EnemyKilledEvent);
}


void ABrawlerCharacter::MoveForward(const float Amount)
{
    if(IsDead() || Amount == 0) return;
    
    const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X)
                            + FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Z);
    AddMovementInput(Direction, Amount / (IsAttacking() || IsDefending() ? 1000 : 1));
}

void ABrawlerCharacter::MoveRight(const float Amount)
{
    if(IsDead() || Amount == 0) return;
    
    const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    AddMovementInput(Direction, Amount * (IsAttacking() || IsDefending() ? 0.0001 : 1));
}

void ABrawlerCharacter::UpdateWalkingFX() const
{
    // Only show step VFX when the player is moving and not falling.
    const bool ShouldShowFX = !IsDead() && GetVelocity().SizeSquared() > 10 && !CharacterMovementComponent->IsFalling();
    if (ShouldShowFX && ParticleSystemComponent->bWasDeactivated)
        ParticleSystemComponent->ActivateSystem();
    else if (!ShouldShowFX && !ParticleSystemComponent->bWasDeactivated)
        ParticleSystemComponent->DeactivateSystem();
}


#pragma region Player Events
void ABrawlerCharacter::TakeDamageEvent(const int& Amount)
{
    // Make sure the player isn't dead or invincible before taking damage.
    if (IsDead() || IsInvincible()) return;

    // Take damage and show damage particles.
    Health -= Amount;
    UNiagaraFunctionLibrary::SpawnSystemAttached(BloodSplatterEffect, this->RootComponent, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);

    // If the character is dead, call the death event.
    if (IsDead()) {
        DeathEvent();
        
        // If the character is an enemy, call the player's enemy killed event.
        if (IsEnemy()) {
            TArray<AActor*> FoundActors;
            UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Player", FoundActors);
            Cast<ABrawlerCharacter>(FoundActors[0])->EnemyKilledEvent();
        }
    }

    // If the character isn't dead start a new invincibility timer.
    else {
        StartInvincibilityEvent();
        DebugInfo("%s hit, lost %d HP, %d HP remaining", *GetName(), Amount, Health);
    }
}

void ABrawlerCharacter::StartAttackingEvent()
{
    // Make sure the player isn't dead and has a weapon before attacking.
    if (IsDead() || !HasEquipment(Weapon)) return;

    // If the player is currently attacking or defending, start the attack buffer.
    if (IsAttacking() || IsDefending()) { AttackBuffer = AttackBufferDuration; return; }
    
    Attacking = true;
    AttackBuffer = 0;
    AttackBlocked = false;
    DebugInfo("%s is attacking.", *GetName());
}

void ABrawlerCharacter::StopAttackingEvent()
{
    if(!IsAttacking()) return;

    Attacking = false;
    AttackBlocked = false;
    DebugInfo("%s is attacking.", *GetName());
}

void ABrawlerCharacter::AttackBlockedEvent()
{
    if (!IsAttacking()) return;

    AttackBlocked = true;
    DebugInfo("%s's attack was blocked.", *GetName());
}

void ABrawlerCharacter::StartDefendingEvent()
{
    // Make sure the player isn't dead and has a shield before defending.
    if (IsDead() || !HasEquipment(Shield)) return;

    // If the player is currently attacking or defending, start the defense buffer.
    if (IsAttacking() || IsDefending()) { DefenseBuffer = DefenseBufferDuration; return; }
    
    Defending = true;
    DefenseBuffer = 0;
    DebugInfo("%s started defending.", *GetName());
}

void ABrawlerCharacter::StopDefendingEvent()
{
    DefenseBuffer = 0;
    if (!IsDefending()) return;
    
    Defending = false;
    DebugInfo("%s stopped defending.", *GetName());
}

void ABrawlerCharacter::StartInvincibilityEvent()
{
    InvincibilityTimer = InvincibilityDuration;
}

void ABrawlerCharacter::DeathEvent()
{
    Health = 0;
    ParticleSystemComponent->DeactivateSystem();
    if (Controller && IsEnemy()) Controller->UnPossess();
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
    
    DebugInfo("%s is dead!", *GetName());
}

void ABrawlerCharacter::EnemyKilledEvent()
{
    KillCount++;
    DebugData("KillCount: %d", KillCount);

    if(!GameMode) return;
    
    AUnrealBrawlerGameModeBase* BrawlerMode = Cast<AUnrealBrawlerGameModeBase>(GameMode);
    UGameHUD* HUD = BrawlerMode->GetGameHUD();
    if(HUD) HUD->UpdateCounterEvent(FString::FromInt(KillCount));
}

void ABrawlerCharacter::DropEquipmentEvent(const EEquipmentType& EquipmentType)
{
    // Get the character's specified equipment piece if they have one.
    AEquipmentActor* EquipmentPiece = nullptr;
    for (AEquipmentActor* EquipmentItem : Equipment) {
        if (EquipmentItem->GetType() == EquipmentType) {
            EquipmentPiece = EquipmentItem;
            break;
        }
    }

    if (EquipmentPiece) {
        EquipmentPiece->GetDropped(this);
        Equipment.Remove(EquipmentPiece);
        DebugInfo("%s dropped equipment piece.", *GetName());
    }
}

void ABrawlerCharacter::PickupEquipmentEvent(AEquipmentActor* NewEquipment)
{
    if (!HasEquipment(NewEquipment->GetType()))
        Equipment.Add(NewEquipment);
}
#pragma endregion


#pragma region Getter & Setter
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
    return Attacking;
}

bool ABrawlerCharacter::WasAttackBlocked() const
{
    return AttackBlocked;
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

bool ABrawlerCharacter::HasEquipment(const EEquipmentType& EquipmentType) const
{
    bool HasSpecifiedEquipment = false;
    for (const AEquipmentActor* EquipmentItem : Equipment) {
        if (EquipmentItem->GetType() == EquipmentType) {
            HasSpecifiedEquipment = true;
            break;
        }
    }
    return HasSpecifiedEquipment;
}
#pragma endregion
