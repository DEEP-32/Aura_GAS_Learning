// 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "AuraAbilitySystemComponent.generated.h"
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetsTags, const FGameplayTagContainer& /* Asset tags*/);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent {
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();
	FEffectAssetsTags EffectAssetsTags;
protected:
	void EffectApplied(UAbilitySystemComponent* AffectingComponent, const FGameplayEffectSpec& Spec,FActiveGameplayEffectHandle Handle);
};
