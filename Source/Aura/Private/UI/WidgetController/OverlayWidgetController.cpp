// 


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/KismetSystemLibrary.h"

void UOverlayWidgetController::BroadcastInitialValues() {
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());

	/*GEngine->AddOnScreenDebugMessage(
		1,
		10,
		FColor::Blue,
		FString::Printf(TEXT("Broadcasted Health value : %f"),AuraAttributeSet->GetHealth())
	);*/

	UE_LOG(LogTemp,Warning,TEXT("Broadcasted Health value : %f"),AuraAttributeSet->GetHealth());
	UE_LOG(LogTemp,Warning,TEXT("Broadcasted Max Health value : %f"),AuraAttributeSet->GetMaxHealth());
	
	/*GEngine->AddOnScreenDebugMessage(
		1,
		10,
		FColor::Blue,
		FString::Printf(TEXT("Broadcasted Max Health value : %f"),AuraAttributeSet->GetMaxHealth())
		
	);*/


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
				  OnMaxHealthChanged.Broadcast(OldMaxHealth.NewValue);
			  }		
		);

	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& OldMana) {
				OnManaChanged.Broadcast(OldMana.NewValue);

				UE_LOG(LogTemp,Warning,TEXT("Broadcasted Mana value : %f"),OldMana.NewValue);
			}		
		);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
		.AddLambda(
			[this](const FOnAttributeChangeData& OldMaxMana) {
				OnMaxManaChanged.Broadcast(OldMaxMana.NewValue);

				UE_LOG(LogTemp,Warning,TEXT("Broadcasted Max Mana value : %f"),OldMaxMana.NewValue)
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


