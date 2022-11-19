#include "BrawlerCharacter.h"

#include "BrawlerInstance.h"
#include "AI/EnemyAiController.h"
#include "Utils/DebugUtils.h"
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

    //HealthComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
}

void ABrawlerCharacter::BeginPlay()
{
    Super::BeginPlay();

    BrawlerGameMode = Cast<AUnrealBrawlerGameModeBase>(GetWorld()->GetAuthGameMode());
    Cast<UBrawlerInstance>(GetWorld()->GetGameInstance())->SetBrawlerCaracter(this);
    
    // Update camera parameters.
    SpringArmComponent->CameraLagSpeed = CameraLag;
    SpringArmComponent->TargetArmLength = CameraDistance;
    
    // If the character is controller by AI, treat this character as an enemy.
    if (AEnemyAiController* Ai = Cast<AEnemyAiController>(GetController()))
    {
        CharacterIsPlayer = false;
        Health = EnemyMaxHealth;
        Tags.Add("Enemy");
		GetMesh()->SetCustomDepthStencilValue(ENEMY_STENCIL_VAL);

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
        {
            if (EquipmentPiece) Cast<AEquipmentActor>(GetWorld()->SpawnActor(EquipmentPiece))->GetPickedUp(this);
        }

        if(EnemyMaxHealth > 1) SetActorScale3D(GetActorScale()+FVector(EnemyMaxHealth*0.05f));
    }

    // If the character is controlled by the player, treat this character as a player.
    else
    {
        // Remove de health bar displayed on top of head, since already showed in HUD
        // HealthBarComponent->DestroyComponent();
        
        Health = PlayerMaxHealth;
        SetActorLabel("Player");
        Tags.Add("Player");
		GetMesh()->SetCustomDepthStencilValue(PLAYER_STENCIL_VAL);

        // Give default equipment to the player.
        for (TSubclassOf<AEquipmentActor> EquipmentPiece : PlayerDefaultEquipment)
            if (EquipmentPiece) Cast<AEquipmentActor>(GetWorld()->SpawnActor(EquipmentPiece))->GetPickedUp(this);
    }

    /*if(IsValid(HealthBarWidgetClass))
    {
        HealthBarComponent = CreateWidget<UHealthBarComponent>(GetWorld(), HealthBarWidgetClass);
        HealthBarComponent->SetupHealthComponent(this);
    }*/
    
    if(IsValid(KillCounterWidgetClass))
    {
        KillCounterComponent = CreateWidget<UKillCounterComponent>(GetWorld(), KillCounterWidgetClass);

        if(IsValid(KillCounterComponent))
        {
            KillCounterComponent->SetupCounterComponent(this);
            KillCounterComponent->AddToViewport();
            KillCounterComponent->SetPositionInViewport(FVector2D(50,50));
            //KillCounterComponent->SetAnchorsInViewport(FAnchors());
        }
    }
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

    // Attack/defend whenever possible if buffers are non-zero.
    const float MaxBuffer = FMath::Max3(RollBuffer, DefenseBuffer, AttackBuffer);
    const bool  IsntDoingAnything = !IsAttacking() && !IsDefending() && !IsRolling();
    if (MaxBuffer == RollBuffer    && RollBuffer    > 0 && IsntDoingAnything) StartRollingEvent  ();
    if (MaxBuffer == DefenseBuffer && DefenseBuffer > 0 && IsntDoingAnything) StartDefendingEvent();
    if (MaxBuffer == AttackBuffer  && AttackBuffer  > 0 && IsntDoingAnything) StartAttackingEvent();

    // Decrement buffers.
    if (AttackBuffer  > 0) AttackBuffer  -= DeltaSeconds;
    if (DefenseBuffer > 0) DefenseBuffer -= DeltaSeconds;
    if (RollBuffer    > 0) RollBuffer    -= DeltaSeconds;

    // Decrement the invincibility timer and call the stop invincibility event when it is finished.
    if (InvincibilityTimer > 0) {
        InvincibilityTimer -= DeltaSeconds;
        if (InvincibilityTimer <= 0) StopInvincibilityEvent();
    }

    // Add velocity to the player if he is rolling.
    if (IsRolling()) {
        GetCharacterMovement()->Launch(GetActorForwardVector() * RollVelocity);
    }
}

void ABrawlerCharacter::UpdateWalkingFX() const
{
    // Only show step VFX when the player is moving and not falling.
    const bool ShouldShowFX = !IsDead() && GetVelocity().SizeSquared() > 10 && !CharacterMovementComponent->IsFalling();
    
    if (ShouldShowFX && WalkingParticleComponent->bWasDeactivated)        WalkingParticleComponent->ActivateSystem();
    else if (!ShouldShowFX && !WalkingParticleComponent->bWasDeactivated) WalkingParticleComponent->DeactivateSystem();
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
    UNiagaraFunctionLibrary::SpawnSystemAttached(BloodSplatterEffect, this->RootComponent, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);

    // Update UI according to new health value
    /*BrawlerGameMode->GetUserInterface()->GetHealthBar()->UpdateCurrentHealthTextEvent(FString::FromInt(GetHealth()));
    BrawlerGameMode->GetUserInterface()->GetHealthBar()->UpdateHealthEvent((float)GetHealth()/GetMaxHealth());*/
    
    // If the character is dead, call the death event.
    if (IsDead())
    {
        DeathEvent();
        
        // If the character is an enemy, call the player's enemy killed event.
        if (IsEnemy())
        {
            TArray<AActor*> FoundActors;
            UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Player", FoundActors);
            Cast<ABrawlerCharacter>(FoundActors[0])->EnemyKilledEvent();
        }
    }

    // If the character isn't dead start a new invincibility timer.
    else
    {
        StartInvincibilityEvent();
        //DebugInfo("%s hit, lost %d HP, %d HP remaining", *GetName(), Amount, Health);
    }
}

void ABrawlerCharacter::StartAttackingEvent()
{
    // Make sure the player isn't dead and has a weapon before attacking.
    if (IsDead() || !HasEquipment(Weapon)) return;

    // If the player is currently attacking, defending or rolling, start the attack buffer.
    if (IsAttacking() || IsDefending() || IsRolling()) { AttackBuffer = AttackBufferDuration; return; }
    
    Attacking = true;
    AttackBuffer = 0;
    AttackBlocked = false;
    //DebugInfo("%s is attacking.", *GetName());
}

void ABrawlerCharacter::StopAttackingEvent()
{
    if(!IsAttacking()) return;

    Attacking = false;
    AttackBlocked = false;
    //DebugInfo("%s is attacking.", *GetName());
}

void ABrawlerCharacter::AttackBlockedEvent()
{
    if (!IsAttacking()) return;

    AttackBlocked = true;
    //DebugInfo("%s's attack was blocked.", *GetName());
}

void ABrawlerCharacter::StartDefendingEvent()
{
    // Make sure the player isn't dead and has a shield before defending.
    if (IsDead() || !HasEquipment(Shield)) return;

    // If the player is currently attacking, defending or rolling, start the defense buffer.
    if (IsAttacking() || IsDefending() || IsRolling()) { DefenseBuffer = DefenseBufferDuration; return; }
    
    Defending = true;
    DefenseBuffer = 0;
    //DebugInfo("%s started defending.", *GetName());
}

void ABrawlerCharacter::StopDefendingEvent()
{
    DefenseBuffer = 0;
    if (!IsDefending()) return;
    
    Defending = false;
    //DebugInfo("%s stopped defending.", *GetName());
}

void ABrawlerCharacter::StartRollingEvent()
{
    // Make sure the player isn't dead before rolling.
    if (IsDead()) return;

    // If the player is currently attacking, defending or rolling, start the roll buffer.
    if (IsAttacking() || IsDefending() || IsRolling()) { RollBuffer = RollBufferDuration; return; }
    
    Rolling = true;
    RollBuffer = 0;
    StartInvincibilityEvent(RollInvincibilityDuration);
    DebugInfo("%s started rolling.", *GetName());
}

void ABrawlerCharacter::StopRollingEvent()
{
    if (!IsRolling()) return;

    Rolling = false;;
    DebugInfo("%s stopped rolling.", *GetName());
}

void ABrawlerCharacter::StartInvincibilityEvent(const float& Duration)
{
    if (!IsPlayer()) return;
    if (Duration == -1)
        InvincibilityTimer = InvincibilityDuration;
    else
        InvincibilityTimer = Duration;

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
    WalkingParticleComponent->DeactivateSystem();
    GetMesh()->SetRenderCustomDepth(false);
    for (const AEquipmentActor* EquipmentPiece : Equipment)
        EquipmentPiece->GetMesh()->SetRenderCustomDepth(false);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);

    if (IsPlayer()) BrawlerGameMode->GetUserInterface()->ShowOverMenuEvent();
    else if (Controller && IsEnemy()) {
        Cast<AEnemyAiController>(Controller)->GetBlackboard()->SetValueAsBool("ShouldAttack", false);
        Controller->UnPossess();
    }
    
    //DebugInfo("%s is dead!", *GetName());
}

void ABrawlerCharacter::EnemyKilledEvent()
{
    KillCount++;
    //DebugData("KillCount: %d", KillCount);
    
    //if(IsValid(BrawlerGameMode)) BrawlerGameMode->GetUserInterface()->GetCounter()->UpdateCounterEvent(FString::FromInt(GetKillCount()));

    // TODO : Replace with spawning system and enemies count alive
    // if(KillCount == 5 && IsPlayer()) BrawlerGameMode->GetUserInterface()->ShowWinMenuEvent();
}

void ABrawlerCharacter::DropEquipmentEvent(const EEquipmentType& EquipmentType)
{
    // Get the character's specified equipment piece if they have one.
    AEquipmentActor* EquipmentPiece = nullptr;
    for (AEquipmentActor* EquipmentItem : Equipment) {
        if (EquipmentItem->GetType() == EquipmentType) { EquipmentPiece = EquipmentItem; break; }
    }

    if (EquipmentPiece) {
        EquipmentPiece->GetDropped(this);
        Equipment.Remove(EquipmentPiece);
        //DebugInfo("%s dropped equipment piece.", *GetName());
    }
}

void ABrawlerCharacter::PickupEquipmentEvent(AEquipmentActor* NewEquipment)
{
    if (!HasEquipment(NewEquipment->GetType())) Equipment.Add(NewEquipment);
}

void ABrawlerCharacter::OpenPauseMenu()
{
    if(BrawlerGameMode->GetUserInterface()->GetPauseMenu()->IsVisible()) return;
    
    BrawlerGameMode->GetUserInterface()->ShowPauseMenuEvent();
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

/*UWidgetComponent* ABrawlerCharacter::GetHealthComponent() const
{
    if(IsValid(HealthComponent)) return HealthComponent;
    return nullptr;
}

UHealthBarComponent* ABrawlerCharacter::GetHealthBarComponent() const
{
    return IsValid(HealthBarComponent) ? HealthBarComponent : nullptr;
}*/
#pragma endregion
