// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlerPlayer.h"
#include "DebugUtils.h"

ABrawlerPlayer::ABrawlerPlayer() {
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

void ABrawlerPlayer::BeginPlay() {
	ParticleSystemComponent->DeactivateSystem();
	
	Super::BeginPlay();

	{
		FTimerHandle FootStepParticles;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ParticleDisplay");
		GetWorld()->GetTimerManager().SetTimer(FootStepParticles, Delegate, 0.2, true);
	}
	
	DebugWarning("Using ABrawlerPlayer");
}

void ABrawlerPlayer::Tick(float _DeltaTime) {
	Super::Tick(_DeltaTime);
}

void ABrawlerPlayer::SetupPlayerInputComponent(class UInputComponent* _InputComponent) {
	Super::SetupPlayerInputComponent(_InputComponent);

	_InputComponent->BindAxis("MoveForward", this, &ABrawlerPlayer::MoveForward);
	_InputComponent->BindAxis("MoveRight", this, &ABrawlerPlayer::MoveRight);

	_InputComponent->BindAxis("LookRight", this, &ABrawlerPlayer::AddControllerYawInput);
	_InputComponent->BindAxis("LookUp", this, &ABrawlerPlayer::AddControllerPitchInput);

	// _inputComponent->BindAction("TakeDamage", IE_Pressed, this, &ABrawlerPlayer::TakeDamage);
	{
		FInputActionBinding TakeDamageBinding("TestKey", IE_Pressed);
		TakeDamageBinding.ActionDelegate.GetDelegateForManualSet().BindLambda([this] { TakeDamage(1); });
		_InputComponent->AddActionBinding(TakeDamageBinding);
	}
}


void ABrawlerPlayer::MoveRight(float _Value) {
	if(IsDead()) return;

	MoveRightDelta = _Value;
	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, MoveRightDelta);
}


void ABrawlerPlayer::MoveForward(float _Value) {
	if(IsDead()) return;

	MoveForwardDelta = _Value;
	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X)
							+ FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Z);
	AddMovementInput(Direction, MoveForwardDelta);
}

void ABrawlerPlayer::TakeDamage(int _Value) {
	playerLife -= _Value;

	if (playerLife <= 0) {
		PlayerDeathEvent();
		Debug("Player is dead!", _Value);
	}
	else {
		Debug("Player hit, lost: %d HP", _Value);
	}
}

bool ABrawlerPlayer::IsDead() {
	return playerDead;
}

void ABrawlerPlayer::PlayerDeathEvent() {
	playerDead = true;
}

void ABrawlerPlayer::ParticleDisplay() {
	if(IsDead()) return;
	
	if(GetMoveForward() == 0 && GetMoveRight() == 0) return ShouldActivateParticle(false);
	if(CharacterMovementComponent->IsFalling() && CharacterMovementComponent->JumpZVelocity >= 0) ShouldActivateParticle(false);

	return ShouldActivateParticle(true);
}

float ABrawlerPlayer::GetMoveForward()
{
	return MoveForwardDelta;
}

float ABrawlerPlayer::GetMoveRight()
{
	return MoveRightDelta;
}

void ABrawlerPlayer::ShouldActivateParticle(bool _Active)
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


