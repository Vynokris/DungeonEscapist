


#include "GameHUD.h"

#include "DebugUtils.h"
#include "Components/TextBlock.h"

bool UGameHUD::Initialize()
{
    const bool Success = Super::Initialize();
    DebugInfo("Created HUD: %d", Success);
    if(!Success) return false;

    if(EnemyCounter)
    {
        EnemyCounter->TextDelegate.BindUFunction(this, "UpdateCounterEvent");
        UpdateCounterEvent("0");
    }
    
    return true;
}

void UGameHUD::NativeConstruct()
{
    Super::NativeConstruct();
}

void UGameHUD::NativeDestruct()
{
    Super::NativeDestruct();
}

void UGameHUD::UpdateCounterEvent(const FString& Amount)
{
    EnemyCounter->SetText(FText::FromString(Amount));
    DebugError("Updated text");
}


