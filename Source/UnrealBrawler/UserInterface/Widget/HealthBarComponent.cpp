


#include "HealthBarComponent.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UnrealBrawler/BrawlerCharacter.h"

#pragma region Setup
bool UHealthBarComponent::Initialize()
{
    if(!Super::Initialize()) return false;
    
    return true;
}

void UHealthBarComponent::NativeConstruct()
{
    Super::NativeConstruct();
}

void UHealthBarComponent::NativeDestruct()
{
    Super::NativeDestruct();
}

void UHealthBarComponent::SetupHealthComponent(AActor* Actor)
{
    if(!IsValid(Actor)) return;
    const ABrawlerCharacter* BrawlerCharacter = Cast<ABrawlerCharacter>(Actor);
    
    if(IsValid(CurrentHealthText)) CurrentHealthText->SetText(FText::AsNumber(BrawlerCharacter->GetHealth()));
    if(IsValid(TotalHealthText))   TotalHealthText  ->SetText(FText::AsNumber(BrawlerCharacter->GetMaxHealth()));
    if(IsValid(HealthBar))         HealthBar        ->SetPercent((float)BrawlerCharacter->GetHealth()/BrawlerCharacter->GetMaxHealth());

    if(BrawlerCharacter->IsEnemy())
    {
        GetHealthBar()->SetFillColorAndOpacity(FLinearColor(255,0,0));
        if (IsValid(BrawlerCharacter->GetHealthBarWidgetComponent()))
            BrawlerCharacter->GetHealthBarWidgetComponent()->SetWidget(this);
    }
}
#pragma endregion


#pragma region Events
void UHealthBarComponent::UpdateHealthEvent(AActor* Actor, const int& Current, const int& Max)
{
    if(!IsValid(Actor)) return;
    const ABrawlerCharacter* BrawlerCharacter = Cast<ABrawlerCharacter>(Actor);
    
    if(IsValid(HealthBar)) HealthBar->SetPercent((float)Current/Max);
    if(IsValid(CurrentHealthText)) CurrentHealthText->SetText(FText::AsNumber(Current));
    
    if(BrawlerCharacter->IsEnemy()) BrawlerCharacter->GetHealthBarWidgetComponent()->SetWidget(this);
}
#pragma endregion


#pragma region Getter & Setter
UProgressBar* UHealthBarComponent::GetHealthBar() const
{
    return IsValid(HealthBar) ? HealthBar : nullptr;
}

UTextBlock* UHealthBarComponent::GetCurrentHealthText() const
{
    return IsValid(CurrentHealthText) ? CurrentHealthText : nullptr;
}

UTextBlock* UHealthBarComponent::GetTotalHealthText() const
{
    return IsValid(TotalHealthText) ? TotalHealthText : nullptr;
}
#pragma endregion
