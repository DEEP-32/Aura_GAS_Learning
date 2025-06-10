// 


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet() {
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AffectingComponent,
                                                const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle) {
	FGameplayTagContainer TagContainer;
	Spec.GetAllAssetTags(TagContainer);

	//EffectA.Broadcast(TagContainer);
	EffectAssetsTags.Broadcast(TagContainer);
}
