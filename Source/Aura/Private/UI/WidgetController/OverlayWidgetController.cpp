// 


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues() {
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies() {
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddUObject(
		this,
		&UOverlayWidgetController::HealthChanged
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(
		this,
		&UOverlayWidgetController::MaxHealthChanged
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddUObject(
		this,
		&UOverlayWidgetController::ManaChanged
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddUObject(
		this,
		&UOverlayWidgetController::MaxManaChanged
	);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& OldMaxHealth) const {
	OnMaxHealthChanged.Broadcast(OldMaxHealth.NewValue);
}
void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& OldHealth) const {
	OnHealthChanged.Broadcast(OldHealth.NewValue);
}
void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& OldMana) const {
	OnManaChanged.Broadcast(OldMana.NewValue);
}
void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& OldMaxMana) const {
	OnMaxManaChanged.Broadcast(OldMaxMana.NewValue);
}
