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

	AuraASC->EffectAssetsTags.AddLambda(
	[](const FGameplayTagContainer& AssetTags) {
		
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

void UOverlayWidgetController::OnEffectAssetTagsRecieved(const FGameplayTagContainer& AssetTags) {

	for (const FGameplayTag& Tag : AssetTags) {
		//TODO : Broadcast the tag to widget controller.

		const FString TagString = FString::Printf(TEXT("Get Tag : %s"), *Tag.ToString());
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Blue,TagString);
	}
}
