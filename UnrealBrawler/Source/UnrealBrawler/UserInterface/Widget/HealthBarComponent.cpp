


#include "HealthBarComponent.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UnrealBrawler/BrawlerCharacter.h"
#include "UnrealBrawler/Utils/DebugUtils.h"

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

void UHealthBarComponent::SetupHealthComponent(const AActor* Actor)
{
    if(IsValid(Actor)) {
        
        const ABrawlerCharacter* BrawlerCharacter = Cast<ABrawlerCharacter>(Actor);
        if(IsValid(HealthBar)) HealthBar->SetPercent((float)BrawlerCharacter->GetHealth()/BrawlerCharacter->GetMaxHealth());
        if(IsValid(CurrentHealthText)) CurrentHealthText->SetText(FText::AsNumber(BrawlerCharacter->GetHealth()));
        if(IsValid(TotalHealthText)) TotalHealthText->SetText(FText::AsNumber(BrawlerCharacter->GetMaxHealth()));
        
        if(BrawlerCharacter->IsPlayer())
        {
            AddToViewport();
            SetPositionInViewport(FVector2D(50,50));
            SetVisibility(ESlateVisibility::Hidden);
            return;
        }

        //BrawlerCharacter->GetHealthComponent()->AttachToComponent(BrawlerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        
        GetCurrentHealthText()->SetVisibility(ESlateVisibility::Hidden);
        GetTotalHealthText  ()->SetVisibility(ESlateVisibility::Hidden);
        GetSeparatorText    ()->SetVisibility(ESlateVisibility::Hidden);
        HealthBar->SetFillColorAndOpacity(FLinearColor(255,0,0));
    }
}
#pragma endregion


#pragma region Events
void UHealthBarComponent::UpdateHealthEvent(const float& Amount)
{
    if(IsValid(HealthBar)) HealthBar->SetPercent(Amount);
    if(IsValid(CurrentHealthText)) CurrentHealthText->SetText(FText::AsNumber(Amount));
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

UTextBlock* UHealthBarComponent::GetSeparatorText() const
{
    return IsValid(SeparatorText) ? SeparatorText : nullptr;
}
#pragma endregion
