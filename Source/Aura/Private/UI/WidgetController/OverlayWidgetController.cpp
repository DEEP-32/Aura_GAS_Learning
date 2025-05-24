// 


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
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
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& OldHealth) {
				OnHealthChanged.Broadcast(OldHealth.NewValue);
			}		
        );

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
		.AddLambda(
			  [this](const FOnAttributeChangeData& OldMaxHealth) {
				  OnHealthChanged.Broadcast(OldMaxHealth.NewValue);
			  }		
		);

	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& OldMana) {
				OnHealthChanged.Broadcast(OldMana.NewValue);
			}		
		);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
		.AddLambda(
				[this](const FOnAttributeChangeData& OldMaxMana) {
					OnHealthChanged.Broadcast(OldMaxMana.NewValue);
				}		
		);

	AuraASC->EffectAssetsTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags) {
			for (const FGameplayTag& Tag : AssetTags) {
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag)) {
					FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);

	//AuraASC->EffectAssetsTags.AddUObject()
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

