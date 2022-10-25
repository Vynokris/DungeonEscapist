// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "BrawlerPlayer.generated.h"

UCLASS()
class UNREALBRAWLER_API ABrawlerPlayer : public ACharacter
{
	GENERATED_BODY()

private:
	USceneComponent* RootComponent;

public:
	UPROPERTY(EditAnywhere) float cameraLag = 15.f;
	UPROPERTY(EditAnywhere) float cameraDistance = 300.f;
	UPROPERTY(EditAnywhere) int playerLife = 5;

	UCameraComponent* CameraComponent;
	USpringArmComponent* SpringArmComponent;
	UCharacterMovementComponent* CharacterMovementComponent;

public:
	ABrawlerPlayer();

	virtual void Tick(float _deltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* _inputComponent) override;

	void MoveRight(float _value);
	void MoveForward(float _value);

	void TakeDamage(int _value);
	bool isAlive();

protected:
	virtual void BeginPlay() override;
};
