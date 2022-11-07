// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlerPlayer.h"
#include "DebugUtils.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

ABrawlerPlayer::ABrawlerPlayer()
{
	bUseControllerRotationYaw = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = cameraLag;
	SpringArmComponent->TargetArmLength = cameraDistance;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = true;

	CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->bUseControllerDesiredRotation = false;
	CharacterMovementComponent->bOrientRotationToMovement = true;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
}

void ABrawlerPlayer::BeginPlay()
{
	ParticleSystemComponent->DeactivateSystem();
	
	SpringArmComponent->CameraLagSpeed = cameraLag;
	SpringArmComponent->TargetArmLength = cameraDistance;
	
	{
		FTimerHandle FootStepParticles;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "DisplayParticle");
		GetWorld()->GetTimerManager().SetTimer(FootStepParticles, Delegate, stepsParticlesRate, true);
	}
	
	DebugWarning("Using ABrawlerPlayer");
	
	Super::BeginPlay();
}

void ABrawlerPlayer::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void ABrawlerPlayer::SetupPlayerInputComponent(UInputComponent* _InputComponent)
{
	Super::SetupPlayerInputComponent(_InputComponent);

	_InputComponent->BindAxis("MoveForward", this, &ABrawlerPlayer::MoveForward);
	_InputComponent->BindAxis("MoveRight", this, &ABrawlerPlayer::MoveRight);

	_InputComponent->BindAxis("LookRight", this, &ABrawlerPlayer::AddControllerYawInput);
	_InputComponent->BindAxis("LookUp", this, &ABrawlerPlayer::AddControllerPitchInput);

	// _inputComponent->BindAction("TakeDamage", IE_Pressed, this, &ABrawlerPlayer::TakeDamage);
	{
		FInputActionBinding TakeDamageBinding("TestKey", IE_Pressed);
		TakeDamageBinding.ActionDelegate.GetDelegateForManualSet().BindLambda([this] { EnemyKilledEvent(); });
		_InputComponent->AddActionBinding(TakeDamageBinding);
	}
}


void ABrawlerPlayer::MoveRight(float _Value)
{
	if(IsDead()) return;

	moveRightDelta = _Value;
	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, moveRightDelta);
}

void ABrawlerPlayer::MoveForward(float _Value)
{
	if(IsDead()) return;

	moveForwardDelta = _Value;
	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X) + FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Z);
	AddMovementInput(Direction, moveForwardDelta);
}


float ABrawlerPlayer::GetMoveForward() const
{
	return moveForwardDelta;
}

float ABrawlerPlayer::GetMoveRight() const
{
	return moveRightDelta;
}


void ABrawlerPlayer::TakeDamageEvent(int _Value)
{
	playerLife -= _Value;

	if (playerLife <= 0) {
		DeathEvent();
		Debug("Player is dead!", _Value);
	}
	else {
		Debug("Player hit, lost: %d HP", _Value);
	}
}


bool ABrawlerPlayer::IsDead() const
{
	return isPlayerDead;
}

void ABrawlerPlayer::DeathEvent()
{
	ParticleSystemComponent->DeactivateSystem();
	isPlayerDead = true;
}


void ABrawlerPlayer::DisplayParticle() const
{
	if(IsDead()) return;
	
	if(GetMoveForward() == 0 && GetMoveRight() == 0) return ShouldDisplay(false);
	if(CharacterMovementComponent->IsFalling() && CharacterMovementComponent->JumpZVelocity >= 0) return ShouldDisplay(false);

	ShouldDisplay(true);
}

void ABrawlerPlayer::ShouldDisplay(bool _Active) const
{
	if(_Active) {
		if(ParticleSystemComponent->bWasDeactivated) {
			ParticleSystemComponent->ActivateSystem();
		}
		return;
	}
	
	if(!ParticleSystemComponent->bWasDeactivated) {
		ParticleSystemComponent->DeactivateSystem();
	}
}


void ABrawlerPlayer::EnemyKilledEvent()
{
	killedEnemy++;
	GameHudComponent->UpdateEnemyCounterEvent(killedEnemy);
}

int ABrawlerPlayer::GetHealth() const
{
	return playerLife;
}

int ABrawlerPlayer::GetStamina() const
{
	return playerStamina;
}

int ABrawlerPlayer::GetKilledEnemy() const
{
	return killedEnemy;
}