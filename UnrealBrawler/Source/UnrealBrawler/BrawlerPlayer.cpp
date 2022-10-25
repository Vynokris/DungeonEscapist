// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlerPlayer.h"
#include "DebugUtils.h"

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

}

void ABrawlerPlayer::BeginPlay()
{
	Super::BeginPlay();

	DebugWarning("Using ABrawlerPlayer");
}

void ABrawlerPlayer::Tick(float _deltaTime)
{
	Super::Tick(_deltaTime);
}

void ABrawlerPlayer::SetupPlayerInputComponent(class UInputComponent* _inputComponent)
{
	Super::SetupPlayerInputComponent(_inputComponent);

	_inputComponent->BindAxis("MoveForward", this, &ABrawlerPlayer::MoveForward);
	_inputComponent->BindAxis("MoveRight", this, &ABrawlerPlayer::MoveRight);

	_inputComponent->BindAxis("LookRight", this, &ABrawlerPlayer::AddControllerYawInput);
	_inputComponent->BindAxis("LookUp", this, &ABrawlerPlayer::AddControllerPitchInput);

	// _inputComponent->BindAction("TakeDamage", IE_Pressed, this, &ABrawlerPlayer::TakeDamage);
	{
		FInputActionBinding TakeDamageBinding("TestKey", IE_Pressed);
		TakeDamageBinding.ActionDelegate.GetDelegateForManualSet().BindLambda([this]() { TakeDamage(1); });
		_inputComponent->AddActionBinding(TakeDamageBinding);
	}
}


void ABrawlerPlayer::MoveRight(float _value) 
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, _value);
}


void ABrawlerPlayer::MoveForward(float _value) 
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X) + FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Z);
	AddMovementInput(Direction, _value);
}

void ABrawlerPlayer::TakeDamage(int _value) {
	playerLife -= _value;

	if (isAlive()) {
		Debug("Player is dead!", _value);
	}
	else {
		Debug("Player hit, lost: %d HP", _value);
	}
}

bool ABrawlerPlayer::isAlive() {
	return playerLife <= 0;
}