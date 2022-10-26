// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Camera/CameraComponent.h"

#include "BrawlerPlayer.generated.h"

UCLASS()
class UNREALBRAWLER_API ABrawlerPlayer : public ACharacter
{
	GENERATED_BODY()

private:
	UCameraComponent* CameraComponent = nullptr;
	USpringArmComponent* SpringArmComponent = nullptr;
	UCharacterMovementComponent* CharacterMovementComponent = nullptr;

	float moveForwardDelta = 0;
	float moveRightDelta = 0;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UParticleSystemComponent* ParticleSystemComponent = nullptr;
	
	UPROPERTY(EditAnywhere) float cameraLag = 15.f;
	UPROPERTY(EditAnywhere) float cameraDistance = 300.f;
	
	UPROPERTY(EditAnywhere) int playerLife = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool isPlayerDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int killedEnemy = 0;

private:
	void ShouldDisplay(bool _Active) const;
	
	void MoveRight(float _Value);
	void MoveForward(float _Value);

	float GetMoveRight() const;
	float GetMoveForward() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* _InputComponent) override;
	
public:
	ABrawlerPlayer();
	
	void TakeDamage(int _Value);
	void PlayerDeathEvent();
	UFUNCTION(BlueprintNativeEvent) void EnemyKilledEvent();
	
	bool IsDead() const;

	UFUNCTION() void DisplayParticle() const;
};
