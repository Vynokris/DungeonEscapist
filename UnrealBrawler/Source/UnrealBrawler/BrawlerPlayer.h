// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameHUD.h"

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
	const FName WeaponSocketName = TEXT("WeaponSocket");
	
	UCameraComponent* CameraComponent = nullptr;
	USpringArmComponent* SpringArmComponent = nullptr;
	UCharacterMovementComponent* CharacterMovementComponent = nullptr;
	UGameHUD* GameHudComponent = nullptr;
	AActor* WeaponInHands = nullptr;
	
	float moveForwardDelta = 0;
	float moveRightDelta = 0;
	
	int killedEnemy = 0;
	
	bool canTakeDamage = true;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite = "Brawler Invincibility") float damageTimer = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UParticleSystemComponent* ParticleSystemComponent = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Brawler View") float cameraLag = 15.f;
	UPROPERTY(EditAnywhere, Category = "Brawler View") float cameraDistance = 300.f;
	
	UPROPERTY(EditAnywhere, Category = "Brawler Stats") int playerLife = 5;
	UPROPERTY(EditAnywhere, Category = "Brawler Stats") int playerStamina = 10;

	UPROPERTY(EditAnywhere, Category = "Brawler VFX") float stepsParticlesRate = 0.2f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Brawler State") bool isPlayerDead = false;

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
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	
public:
	ABrawlerPlayer();

	UFUNCTION(BlueprintCallable) int GetHealth() const;
	UFUNCTION(BlueprintCallable) int GetStamina() const;
	UFUNCTION(BlueprintCallable) int GetKilledEnemy() const;
	
	void TakeDamageEvent(int _Value);
	void DeathEvent();
	void EnemyKilledEvent();
	
	bool IsDead() const;
	
	UFUNCTION() void SetTakeDamage(bool _TakeDamage, bool _WithTimer);

	void StartTakeDamageTimer();
	
	UFUNCTION() void DisplayParticle() const;
};
