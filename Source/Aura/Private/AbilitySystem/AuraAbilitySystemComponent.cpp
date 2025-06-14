// 


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayAbility.h"
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

void UAuraAbilitySystemComponent::AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> Abilities) {

	for (const TSubclassOf<UGameplayAbility> Ability : Abilities) {

		FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(Ability,1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(GameplayAbilitySpec.Ability)) {
			GameplayAbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(GameplayAbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag) {
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& ActivatableAbility : GetActivatableAbilities()) {
		if (ActivatableAbility.DynamicAbilityTags.HasTagExact(InputTag)) {
			AbilitySpecInputPressed(ActivatableAbility);
			if (!ActivatableAbility.IsActive()) {
				TryActivateAbility(ActivatableAbility.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag) {

	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& ActivatableAbility : GetActivatableAbilities()) {
		if (ActivatableAbility.DynamicAbilityTags.HasTagExact(InputTag)) {
			AbilitySpecInputReleased(ActivatableAbility);
		}
	}
	
}
