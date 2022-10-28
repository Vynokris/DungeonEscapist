


#include "GameHUD.h"

#include "DebugUtils.h"
#include "Components/TextBlock.h"

void UGameHUD::NativeConstruct()
{
    Super::NativeConstruct();
}

bool UGameHUD::Initialize()
{
    const bool Success = Super::Initialize();
    if(!Success) return false;

    if(EnemyCounter)
    {
        EnemyCounter->TextDelegate.BindUFunction(this, "SetCounterText");
        Debug("Found EnemyCounter"); 
    }
    return true;
}


void UGameHUD::UpdateEnemyCounterEvent(int _Value)
{
    SetCounterText(_Value);
    Debug("Setting value GUI: ", _Value);
}

FText UGameHUD::SetCounterText(int _Value) const
{
    return FText::FromString(FString::FromInt(_Value));
}

