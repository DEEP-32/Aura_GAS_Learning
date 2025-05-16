// 


#include "AbilitySystem/AuraAbilitySystemComponent.h"


void UAuraAbilitySystemComponent::AbilityActorInfoSet() {
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AffectingComponent,
                                                const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle) {
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Blue, FString("Effect applied!"));
}

