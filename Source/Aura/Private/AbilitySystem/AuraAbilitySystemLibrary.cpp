// 


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(
	const UObject* WorldContextObject) {
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0)) {
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD())) {
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PS->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PS->GetAttributeSet();

			const FWidgetControlledParams Params(PC, PS, AbilitySystemComponent, AttributeSet);

			return AuraHUD->GetOverlayWidgetController(Params);
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject) {
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0)) {
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD())) {
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PS->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PS->GetAttributeSet();

			const FWidgetControlledParams Params(PC, PS, AbilitySystemComponent, AttributeSet);

			return AuraHUD->GetAttributeMenuWidgetController(Params);
		}
	}

	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,ECharacterClass CharacterClass, float Level,UAbilitySystemComponent* ASC) {
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode) {
		return;
	}

	const FCharacterClassDefaultInfo DefaultInfo = AuraGameMode->CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	
	const FGameplayEffectSpecHandle PrimarySpecHandle = ASC->MakeOutgoingSpec(
		DefaultInfo.PrimaryAttributes,
		Level,
		ASC->MakeEffectContext()
	);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimarySpecHandle.Data.Get(),FPredictionKey());

	const FGameplayEffectSpecHandle SecondarySpecHandle = ASC->MakeOutgoingSpec(
		DefaultInfo.SecondaryAttributes,
		Level,
		ASC->MakeEffectContext()
	);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondarySpecHandle.Data.Get(),FPredictionKey());

	const FGameplayEffectSpecHandle VitalSpecHandle = ASC->MakeOutgoingSpec(
		DefaultInfo.VitalAttributes,
		Level,
		ASC->MakeEffectContext()
	);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalSpecHandle.Data.Get(),FPredictionKey());

	
}
