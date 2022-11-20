#include "BrawlerCharacter.h"

#include "AI/EnemyAiController.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/WidgetTree.h"
#include "NiagaraFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UserInterface/Widget/HealthBarComponent.h"
#include "UserInterface/Widget/KillCOunterComponent.h"

#define PLAYER_STENCIL_VAL 1
#define ENEMY_STENCIL_VAL 2
#define INVINCIBILITY_STENCIL_VAL 3

#pragma region Setup 
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
    WalkingParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
    WalkingParticleComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

    // Make an outline around characters.
    GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(0);

    // Setup health bar widget.
    HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComponent"));
    HealthBarWidgetComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}

void ABrawlerCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // Update camera parameters.
    SpringArmComponent->CameraLagSpeed = CameraLag;
    SpringArmComponent->TargetArmLength = CameraDistance;

    UserInterface = Cast<UUserInterfaceManager>(CreateWidget(GetWorld(), GameWidgetHUD));

    // If the character is controlled by the player, treat this character as a player.
    if (IsValid(Controller) && Controller->IsA<APlayerController>())
    {
        // Make the character a player.
        Health = PlayerMaxHealth;
		GetMesh()->SetCustomDepthStencilValue(PLAYER_STENCIL_VAL);
        Tags.Add("Player");
        SetActorLabel("Player");

        // Give default equipment to the player.
        for (TSubclassOf<AEquipmentActor> EquipmentPiece : PlayerDefaultEquipment)
            if (EquipmentPiece) Cast<AEquipmentActor>(GetWorld()->SpawnActor(EquipmentPiece))->GetPickedUp(this);
        
        // Remove the health bar displayed above the player's head, since it is already showed in HUD.
        GetHealthBarWidgetComponent()->DestroyComponent();

        // Add the hud to the player's screen and show the main menu.
        UserInterface->AddToViewport();
        UserInterface->ShowMainMenuEvent();
        UserInterface->GetCounterComponent()->SetupCounterComponent(this);
    }
    
    // If the character is controlled by AI, treat this character as an enemy.
    else
    {
        // Make the character an enemy.
        CharacterIsPlayer = false;
        Health = EnemyMaxHealth;
        GetCharacterMovement()->MaxWalkSpeed = 500;
		GetMesh()->SetCustomDepthStencilValue(ENEMY_STENCIL_VAL);
        Tags.Add("Enemy");

        // Force the enemy to always face the player.
        CharacterMovementComponent->bUseControllerDesiredRotation = true;
        CharacterMovementComponent->bOrientRotationToMovement     = false;
        if (AEnemyAiController* Ai = Cast<AEnemyAiController>(Controller))
            Ai->SetFocus(Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)), EAIFocusPriority::Gameplay);

        // Remove the camera and spring arm.
        GetComponentByClass(UCameraComponent   ::StaticClass())->DestroyComponent();
        GetComponentByClass(USpringArmComponent::StaticClass())->DestroyComponent();
        
        // Give default equipment to the enemy.
        for (TSubclassOf<AEquipmentActor> EquipmentPiece : EnemyDefaultEquipment)
            if (EquipmentPiece) Cast<AEquipmentActor>(GetWorld()->SpawnActor(EquipmentPiece))->GetPickedUp(this);

        // Scale the enemy depending on its health.
        if(EnemyMaxHealth > 1) SetActorScale3D(GetActorScale() + FVector(EnemyMaxHealth * 0.05f));
    }
    
    UserInterface->GetHealthBarComponent()->SetupHealthComponent(this);
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

    // Roll key.
    NewInputComponent->BindAction("Roll", IE_Pressed, this, &ABrawlerCharacter::StartRollingEvent);

    // Pause key.
    NewInputComponent->BindAction("Pause", IE_Pressed, this, &ABrawlerCharacter::OpenPauseMenu);
    
	// Drop keys.
	{
		FInputActionBinding DropWeaponBinding("DropWeapon", IE_Pressed);
		DropWeaponBinding.ActionDelegate.GetDelegateForManualSet().BindLambda([this] { DropEquipmentEvent(Weapon); });
		NewInputComponent->AddActionBinding(DropWeaponBinding);
		
		FInputActionBinding DropShieldBinding("DropShield", IE_Pressed);
		DropShieldBinding.ActionDelegate.GetDelegateForManualSet().BindLambda([this] { DropEquipmentEvent(Shield); });
		NewInputComponent->AddActionBinding(DropShieldBinding);
	}
}
#pragma endregion


#pragma region Update
void ABrawlerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdateWalkingFX();
    UpdateTimers(DeltaSeconds);
    if (IsPlayer())
        UpdateRoll();
}

void ABrawlerCharacter::UpdateWalkingFX() const
{
    // Only show step VFX when the player is moving and not falling.
    const bool ShouldShowFX = !IsDead() && GetVelocity().SizeSquared() > 10 && !CharacterMovementComponent->IsFalling();
    
    if      (ShouldShowFX  &&  WalkingParticleComponent->bWasDeactivated) WalkingParticleComponent->ActivateSystem();
    else if (!ShouldShowFX && !WalkingParticleComponent->bWasDeactivated) WalkingParticleComponent->DeactivateSystem();
}

void ABrawlerCharacter::UpdateTimers(const float& DeltaSeconds)
{
    // Make sure the player isn't doing anything.
    if (!IsAttacking() && !IsDefending() && !IsRolling())
    {
        // Check what action he should perform based on the buffers.
        const float MaxBuffer = FMath::Max((AttackCooldown > 0 ? 0 : AttackBuffer),
                                           (RollCooldown   > 0 ? 0 : RollBuffer  ));
        const bool  ShouldAttack      = (MaxBuffer == AttackBuffer  && AttackBuffer  > 0 && AttackCooldown <= 0);
        const bool  ShouldRoll        = (MaxBuffer == RollBuffer    && RollBuffer    > 0 && RollCooldown <= 0);
        const bool  ShouldDefend      = TryingToDefend && !ShouldAttack && !ShouldRoll;
    
        // Attack/defend whenever possible if buffers are non-zero.
        if      (ShouldAttack) StartAttackingEvent();
        else if (ShouldDefend) StartDefendingEvent();
        else if (ShouldRoll  ) StartRollingEvent  ();
    }

    // Decrement buffers and cooldowns.
    if (AttackBuffer   > 0) AttackBuffer   -= DeltaSeconds;
    if (RollBuffer     > 0) RollBuffer     -= DeltaSeconds;
    if (AttackCooldown > 0) AttackCooldown -= DeltaSeconds;
    if (RollCooldown   > 0) RollCooldown   -= DeltaSeconds;

    // Decrement the invincibility timer and call the stop invincibility event when it is finished.
    if (InvincibilityTimer > 0) {
        InvincibilityTimer -= DeltaSeconds;
        if (InvincibilityTimer <= 0) StopInvincibilityEvent();
    }
}

void ABrawlerCharacter::UpdateRoll() const
{
    // Add velocity to the player if he is rolling.
    if (IsRolling()) {
        GetCharacterMovement()->Launch(GetActorForwardVector() * RollVelocity);
    }
}
#pragma endregion


#pragma region Movement
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
#pragma endregion


#pragma region Player Events
void ABrawlerCharacter::TakeDamageEvent(const int& Amount)
{
    // Make sure the player isn't dead or invincible before taking damage.
    if (IsDead() || IsInvincible()) return;

    // Take damage and show damage particles.
    Health -= Amount;
    UNiagaraFunctionLibrary::SpawnSystemAttached(BloodSplatterEffect,
                                                 this->RootComponent,
                                                 NAME_None,
                                                 FVector(0.f),
                                                 FRotator(0.f),
                                                 EAttachLocation::Type::KeepRelativeOffset,
                                                 true);

    // Update UI according to new health value
    UserInterface->GetHealthBarComponent()->UpdateHealthEvent(this, GetHealth(), GetMaxHealth());
    
    // If the character is dead, call the death event.
    if (IsDead())
    {
        DeathEvent();
        
        // If the character is an enemy, call the player's enemy killed event.
        if (IsEnemy()) {
            TArray<AActor*> FoundActors;
            UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Player", FoundActors);
            Cast<ABrawlerCharacter>(FoundActors[0])->EnemyKilledEvent();
        }
    }

    // If the character isn't dead start a new invincibility timer.
    else
    {
        StartInvincibilityEvent();
    }
}

void ABrawlerCharacter::StartAttackingEvent()
{
    // Make sure the player isn't dead and has a weapon before attacking.
    if (IsDead() || !HasEquipment(Weapon)) return;

    // If the player is currently attacking, defending or rolling, start the attack buffer.
    if (IsAttacking() || IsDefending() || IsRolling() || AttackCooldown > 0) { AttackBuffer = AttackBufferDuration; return; }
    
    Attacking     = true;
    AttackBuffer  = 0;
    AttackBlocked = false;
}

void ABrawlerCharacter::StopAttackingEvent()
{
    if (!IsAttacking()) return;

    Attacking      = false;
    AttackBlocked  = false;
    AttackCooldown = AttackCooldownDuration;
}

void ABrawlerCharacter::AttackBlockedEvent()
{
    if (!IsAttacking()) return;

    AttackBlocked = true;
}

void ABrawlerCharacter::StartDefendingEvent()
{
    // Make sure the player isn't dead and has a shield before defending.
    if (IsDead() || !HasEquipment(Shield)) return;

    // If the player is currently attacking, defending or rolling, start the defense buffer.
    if (IsAttacking() || IsDefending() || IsRolling()) { TryingToDefend = true; AttackBuffer = 0; RollBuffer = 0; return; }
    
    Defending = true;
    TryingToDefend = false;
}

void ABrawlerCharacter::StopDefendingEvent()
{
    TryingToDefend = false;
    if (!IsDefending()) return;
    
    Defending = false;
}

void ABrawlerCharacter::StartRollingEvent()
{
    // Make sure the player isn't dead before rolling.
    if (IsDead()) return;

    // If the player is currently attacking, defending or rolling, start the roll buffer.
    if (IsAttacking() || IsDefending() || IsRolling() || RollCooldown > 0) { RollBuffer = RollBufferDuration; return; }
    
    Rolling    = true;
    RollBuffer = 0;
    StartInvincibilityEvent(RollInvincibilityDuration);
}

void ABrawlerCharacter::StopRollingEvent()
{
    if (!IsRolling()) return;

    Rolling      = false;
    RollCooldown = RollCooldownDuration;
    StopInvincibilityEvent();
}

void ABrawlerCharacter::StartInvincibilityEvent(const float& Duration)
{
    // Give invincibility only to the player.
    if (!IsPlayer()) return;

    // Start the invincibility timer.
    if (Duration == -1) InvincibilityTimer = InvincibilityDuration;
    else                InvincibilityTimer = Duration;

    // Change the outline color of the character and his equipment to the invincibility color.
    GetMesh()->SetCustomDepthStencilValue(INVINCIBILITY_STENCIL_VAL);
    for (const AEquipmentActor* EquipmentPiece : Equipment)
        EquipmentPiece->GetMesh()->SetCustomDepthStencilValue(INVINCIBILITY_STENCIL_VAL);
}

void ABrawlerCharacter::StopInvincibilityEvent()
{
    if (!IsPlayer()) return;
    InvincibilityTimer = 0;
    
    // Reset the outline color of the character and his equipment.
    GetMesh()->SetCustomDepthStencilValue(PLAYER_STENCIL_VAL);
    for (const AEquipmentActor* EquipmentPiece : Equipment)
        EquipmentPiece->GetMesh()->SetCustomDepthStencilValue(PLAYER_STENCIL_VAL);
}

void ABrawlerCharacter::DeathEvent()
{
    Health = 0;
    
    // Disable step particles, outline and collisions.
    WalkingParticleComponent->DeactivateSystem();
    GetMesh()->SetRenderCustomDepth(false);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);

    // Notify equipment pieces about the death of their parent.
    for (const AEquipmentActor* EquipmentPiece : Equipment)
        EquipmentPiece->ParentDeathEvent();

    // If the player died, show the game over menu.
    if (IsPlayer()) UserInterface->ShowOverMenuEvent();

    // If an enemy died, destroy its health bar and un-possess it.
    else if (Controller && IsEnemy()) {
        HealthBarWidgetComponent->DestroyComponent();
        Cast<AEnemyAiController>(Controller)->GetBlackboard()->SetValueAsBool("ShouldAttack", false);
        Controller->UnPossess();
    }
}

void ABrawlerCharacter::EnemyKilledEvent()
{
    KillCount++;
    UserInterface->GetCounterComponent()->UpdateCounterEvent(FText::AsNumber(GetKillCount()));
}

void ABrawlerCharacter::DropEquipmentEvent(const EEquipmentType& EquipmentType)
{
    // Get the character's specified equipment piece if they have one.
    AEquipmentActor* EquipmentPiece = nullptr;
    for (AEquipmentActor* EquipmentItem : Equipment) {
        if (EquipmentItem->GetType() == EquipmentType) { EquipmentPiece = EquipmentItem; break; }
    }

    // Drop the specified equipment piece.
    if (EquipmentPiece) {
        EquipmentPiece->GetDropped(this);
        Equipment.Remove(EquipmentPiece);
    }
}

void ABrawlerCharacter::PickupEquipmentEvent(AEquipmentActor* NewEquipment)
{
    if (!HasEquipment(NewEquipment->GetType())) Equipment.Add(NewEquipment);
}

void ABrawlerCharacter::OpenPauseMenu()
{
    if(UserInterface->GetPauseMenu()->IsVisible()) return;
    UserInterface->ShowPauseMenuEvent();
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

int ABrawlerCharacter::GetMaxHealth() const
{
    return IsPlayer() ? PlayerMaxHealth : EnemyMaxHealth;
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

bool ABrawlerCharacter::IsRolling() const
{
    return Rolling;
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

UWidgetComponent* ABrawlerCharacter::GetHealthBarWidgetComponent() const
{
    return IsValid(HealthBarWidgetComponent) ? HealthBarWidgetComponent : nullptr;
}
#pragma endregion
