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

        BrawlerGameMode = Cast<AUnrealBrawlerGameModeBase>(GetWorld()->GetAuthGameMode());
        if(IsValid(BrawlerGameMode))
        {
            BrawlerGameMode->GetGameHUD()->GetHealthBar()->UpdateCurrentHealthTextEvent(FString::FromInt(GetHealth()));
            BrawlerGameMode->GetGameHUD()->GetHealthBar()->UpdateTotalHealthTextEvent(FString::FromInt(GetHealth()));
            BrawlerGameMode->GetGameHUD()->GetCounter()->UpdateCounterEvent(FString::FromInt(GetKillCount()));
        }

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

    // Decrement timer.
    if (InvincibilityTimer > 0) InvincibilityTimer -= DeltaSeconds;
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
    if (IsDead()) return;
    
    const bool ShouldShowFX = GetVelocity().SizeSquared() > 10 && !CharacterMovementComponent->IsFalling();
    if (ShouldShowFX && ParticleSystemComponent->bWasDeactivated)
        ParticleSystemComponent->ActivateSystem();
    else if (!ShouldShowFX && !ParticleSystemComponent->bWasDeactivated)
        ParticleSystemComponent->DeactivateSystem();
}


#pragma region Player Events
void ABrawlerCharacter::TakeDamageEvent(const int& Amount)
{
    if (IsDead() || IsInvincible()) return;
    
    Health -= Amount;
    UNiagaraFunctionLibrary::SpawnSystemAttached(BloodSplatterEffect, this->RootComponent, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
    
    if (IsDead()) {
        DeathEvent();
        if(IsEnemy())
            EnemyKilledEvent(); // If an enemy was killed that means the player killed him.
    }
    else {
        StartInvincibilityEvent();
        DebugInfo("%s hit, lost %d HP, %d HP remaining", *GetName(), Amount, Health);

        if(!BrawlerGameMode) return;
        if(IsValid(BrawlerGameMode)) BrawlerGameMode->GetGameHUD()->GetHealthBar()->UpdateHealthEvent(Health);
    }
}

void ABrawlerCharacter::StartAttackingEvent()
{
    // Make sure the player isn't dead or defending and has a weapon before attacking.
    if(IsDead() || IsDefending() || !HasEquipment(Weapon)) return;

    Attacking = true;
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
    // Make sure the player isn't dead or attacking and has a shield before defending.
    if (IsDead() || IsAttacking() || !HasEquipment(Shield)) return;
    
    Defending = true;
    DebugInfo("%s started defending.", *GetName());
}

void ABrawlerCharacter::StopDefendingEvent()
{
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
    DebugData("KillCount %d: ", KillCount);

    if(!BrawlerGameMode) return;
    if(IsValid(BrawlerGameMode)) BrawlerGameMode->GetGameHUD()->GetCounter()->UpdateCounterEvent(FString::FromInt(GetKillCount()));
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
