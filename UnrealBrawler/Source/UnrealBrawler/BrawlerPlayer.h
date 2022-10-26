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
	UCameraComponent* CameraComponent = nullptr;
	USpringArmComponent* SpringArmComponent = nullptr;
	UCharacterMovementComponent* CharacterMovementComponent = nullptr;
	
protected:
	
public:
	UPROPERTY(EditAnywhere) float cameraLag = 15.f;
	UPROPERTY(EditAnywhere) float cameraDistance = 300.f;
	
	UPROPERTY(EditAnywhere) int playerLife = 5;
private:
	
protected:
	virtual void BeginPlay() override;
	
public:
	ABrawlerPlayer();

	virtual void Tick(float _DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* _InputComponent) override;

	void MoveRight(float _Value);
	void MoveForward(float _Value);

	void TakeDamage(int _Value);
	bool IsAlive();

	UFUNCTION(BlueprintImplementableEvent, Category = "Brawler VFX") void ParticleDisplay();
};
