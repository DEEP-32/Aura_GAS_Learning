// 


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies() {
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (FAuraAttributeInfo& Info : AttributeInfo.Get()->AttributesInfo) {
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Info.Attribute).AddLambda(
				[this,&Info](const FOnAttributeChangeData& Data){
					BroadcastAttributeInfo(Info);
				}
		);
	} 
}

void UAttributeMenuWidgetController::BroadcastInitialValues() {

	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (FAuraAttributeInfo& AttributesInfo : AttributeInfo.Get()->AttributesInfo) {
		BroadcastAttributeInfo(AttributesInfo);
	} 
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(FAuraAttributeInfo& Info) {
	Info.AttributeValue = Info.Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
