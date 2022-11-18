


#include "HealthBarComponent.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UnrealBrawler/BrawlerCharacter.h"
#include "UnrealBrawler/Utils/DebugUtils.h"

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

        // Setup UI for player and enemies with respective data
        UpdateHealthEvent((float)BrawlerCharacter->GetHealth()/BrawlerCharacter->GetMaxHealth());
        UpdateCurrentHealthTextEvent(FString::FromInt(BrawlerCharacter->GetHealth()));
        UpdateTotalHealthTextEvent(FString::FromInt(BrawlerCharacter->GetMaxHealth()));
        
        
        if(BrawlerCharacter->IsPlayer())
        {
            this->AddToViewport();
            this->SetPositionInViewport(FVector2D(50,50));
            this->SetVisibility(ESlateVisibility::Hidden);
            return;
        }

        BrawlerCharacter->GetHealthComponent()->AttachToComponent(BrawlerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        
        GetCurrentHealthText()->SetVisibility(ESlateVisibility::Hidden);
        GetTotalHealthText  ()->SetVisibility(ESlateVisibility::Hidden);
        GetSeparatorText    ()->SetVisibility(ESlateVisibility::Hidden);
        HealthBar->SetFillColorAndOpacity(FLinearColor(255,0,0));
    }
}

void UHealthBarComponent::UpdateHealthEvent(const float& Amount)
{
    if(IsValid(HealthBar)) HealthBar->SetPercent(Amount);
}

void UHealthBarComponent::UpdateCurrentHealthTextEvent(const FString& Amount)
{
    if(IsValid(CurrentHealthText)) CurrentHealthText->SetText(FText::FromString(Amount));
}

UProgressBar* UHealthBarComponent::GetHealthBar() const
{
    return IsValid(HealthBar) ? HealthBar : nullptr;
}

void UHealthBarComponent::UpdateTotalHealthTextEvent(const FString& Amount)
{
    if(IsValid(TotalHealthText)) TotalHealthText->SetText(FText::FromString(Amount));
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

