// 


#include "AbilitySystem/AuraAbilitySystemComponent.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet() {
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AffectingComponent,
                                                const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle) {
	FGameplayTagContainer TagContainer;
	Spec.GetAllAssetTags(TagContainer);

	for (const FGameplayTag& Tag : TagContainer) {
		//TODO : Broadcast the tag to widget controller.

		const FString TagString = FString::Printf(TEXT("Get Tag : %s"), *Tag.ToString());
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Blue,TagString);
		
	}
}
