#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Actors/EquipmentActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "NiagaraComponent.h"
#include "UnrealBrawlerGameModeBase.h"
#include "Components/WidgetComponent.h"
#include "BrawlerCharacter.generated.h"

UCLASS()
class UNREALBRAWLER_API ABrawlerCharacter : public ACharacter
{
    GENERATED_BODY()

private:
    USpringArmComponent*         SpringArmComponent         = nullptr;
    UCameraComponent*            CameraComponent            = nullptr;
    UCharacterMovementComponent* CharacterMovementComponent = nullptr;
    UUserWidget*				 CurrentHUD					= nullptr;
    UUserInterfaceManager*       UserInterface              = nullptr;
    
    UPROPERTY(EditAnywhere, Category = "Camera") float CameraLag      = 15.f;
    UPROPERTY(EditAnywhere, Category = "Camera") float CameraDistance = 300.f;
    
    UPROPERTY(EditAnywhere,     Category = "Character") int   PlayerMaxHealth           = 5;
    UPROPERTY(EditAnywhere,     Category = "Character") int   EnemyMaxHealth            = 3;
    UPROPERTY(EditAnywhere,     Category = "Character") int   AttackDamage              = 1;
    UPROPERTY(EditAnywhere,     Category = "Character") float RollInvincibilityDuration = 0.5;
    UPROPERTY(EditAnywhere,     Category = "Character") int   InvincibilityDuration     = 2;
    UPROPERTY(EditDefaultsOnly, Category = "Character") TArray<TSubclassOf<AEquipmentActor>> PlayerDefaultEquipment;
    UPROPERTY(EditDefaultsOnly, Category = "Character") TArray<TSubclassOf<AEquipmentActor>> EnemyDefaultEquipment;
    
    UPROPERTY(EditAnywhere, Category = "Input") float AttackBufferDuration   = 1.5;
    UPROPERTY(EditAnywhere, Category = "Input") float RollBufferDuration     = 1.5;
    UPROPERTY(EditAnywhere, Category = "Input") float AttackCooldownDuration = 0.2;
    UPROPERTY(EditAnywhere, Category = "Input") float RollCooldownDuration   = 0.5;
    
    UPROPERTY(EditAnywhere, Category = "VFX") UNiagaraSystem*           BloodSplatterEffect      = nullptr;
    UPROPERTY(EditAnywhere, Category = "VFX") float                     BloodFxSize              = 10.f;
    UPROPERTY(EditAnywhere, Category = "VFX") UParticleSystemComponent* WalkingParticleComponent = nullptr;
    UPROPERTY(EditAnywhere, Category = "VFX") float                     WalkingFxRate            = 0.2f;

    UPROPERTY(EditAnywhere, Category = "UX") UWidgetComponent*          HealthBarWidgetComponent = nullptr;
    UPROPERTY(EditAnywhere, Category = "UX") TSubclassOf<UUserWidget>   GameWidgetHUD;
    
    UPROPERTY(EditAnywhere, Category = "Character Movement: Walking") float MaxEnemyWalkSpeed = 500;
    
    UPROPERTY(EditAnywhere, Category = "Character Movement: Rolling") float RollVelocity = 1000;

    bool  CharacterIsPlayer  = true;
    int   Health             = 0;
    int   KillCount          = 0;
    bool  Attacking          = false; // Turns true when the character is attacking.
    bool  AttackBlocked      = false; // Turns true when the character hits a shield while attacking.
    float AttackBuffer       = 0;     // Starts ticking when the character tries to attack but is unable to.
    float AttackCooldown     = 0;     // Starts ticking when the character stops attacking and prevents starting an attack until it is done.
    bool  Defending          = false; // Turns true when the character is defending.
    bool  TryingToDefend     = false; // Turns to true when the character tries to defend but is unable to.
    bool  Rolling            = false; // Turns true when the character is rolling.
    float RollBuffer         = 0;     // Starts ticking when the character tries to roll but is unable to.
    float RollCooldown       = 0;     // Starts ticking when the character stops rolling and prevents starting an roll until it is done.
    float InvincibilityTimer = 0;

    TArray<AEquipmentActor*> Equipment;
    
private:
    void MoveForward(const float Amount);
    void MoveRight  (const float Amount);

    void UpdateWalkingFX() const;
    void UpdateTimers(const float& DeltaSeconds);
    void UpdateRoll() const;

protected:
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* NewInputComponent) override;
    
public:
    ABrawlerCharacter();
    virtual void BeginPlay() override;
    
    UFUNCTION(BlueprintCallable) void TakeDamageEvent(const int& Amount);
    UFUNCTION(BlueprintCallable) void StartAttackingEvent();
    UFUNCTION(BlueprintCallable) void StopAttackingEvent();
    UFUNCTION(BlueprintCallable) void AttackBlockedEvent();
    UFUNCTION(BlueprintCallable) void StartDefendingEvent();
    UFUNCTION(BlueprintCallable) void StopDefendingEvent();
    UFUNCTION(BlueprintCallable) void StartRollingEvent();
    UFUNCTION(BlueprintCallable) void StopRollingEvent();
    UFUNCTION(BlueprintCallable) void StartInvincibilityEvent(const float& Duration = -1);
    UFUNCTION(BlueprintCallable) void StopInvincibilityEvent();
    UFUNCTION(BlueprintCallable) void DeathEvent();
    UFUNCTION(BlueprintCallable) void EnemyKilledEvent();
    UFUNCTION(BlueprintCallable) void DropEquipmentEvent(const EEquipmentType& EquipmentType);
    UFUNCTION(BlueprintCallable) void PickupEquipmentEvent(AEquipmentActor* NewEquipment);

    UFUNCTION(BlueprintCallable) void OpenPauseMenu();

    UFUNCTION(BlueprintCallable) int  IsPlayer()	     const;
    UFUNCTION(BlueprintCallable) int  IsEnemy()		     const;
    UFUNCTION(BlueprintCallable) int  GetHealth()	     const;
    UFUNCTION(BlueprintCallable) int  GetMaxHealth()     const;
    UFUNCTION(BlueprintCallable) int  GetAttackDamage()  const;
    UFUNCTION(BlueprintCallable) int  GetKillCount()     const;
    UFUNCTION(BlueprintCallable) bool IsAttacking()	     const;
    UFUNCTION(BlueprintCallable) bool WasAttackBlocked() const;
    UFUNCTION(BlueprintCallable) bool IsDefending()	     const;
    UFUNCTION(BlueprintCallable) bool IsInvincible()     const;
    UFUNCTION(BlueprintCallable) bool IsRolling()	     const;
    UFUNCTION(BlueprintCallable) bool IsDead()           const;
    UFUNCTION(BlueprintCallable) bool HasEquipment(const EEquipmentType& EquipmentType) const;

    UFUNCTION(BlueprintCallable) UWidgetComponent* GetHealthBarWidgetComponent() const;
};
