// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Camera/CameraComponent.h"

#include "Components/CapsuleComponent.h"

#include "BrawlerPlayer.generated.h"

UCLASS()
class UNREALBRAWLER_API ABrawlerPlayer : public ACharacter
{
	GENERATED_BODY()

private:
	UCameraComponent* CameraComponent = nullptr;
	USpringArmComponent* SpringArmComponent = nullptr;
	UCharacterMovementComponent* CharacterMovementComponent = nullptr;

	float MoveForwardDelta = 0;
	float MoveRightDelta = 0;
	
protected:
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UParticleSystemComponent* ParticleSystemComponent = nullptr;
	
	UPROPERTY(EditAnywhere) float cameraLag = 15.f;
	UPROPERTY(EditAnywhere) float cameraDistance = 300.f;
	
	UPROPERTY(EditAnywhere) int playerLife = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool playerDead = false;
private:
	void ShouldActivateParticle(bool _Active);
	
protected:
	virtual void BeginPlay() override;
	
public:
	ABrawlerPlayer();

	virtual void Tick(float _DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* _InputComponent) override;

	void MoveRight(float _Value);
	void MoveForward(float _Value);

	float GetMoveRight();
	float GetMoveForward();

	void TakeDamage(int _Value);
	bool IsDead();

	void PlayerDeathEvent();
	
	UFUNCTION() void ParticleDisplay();
};
