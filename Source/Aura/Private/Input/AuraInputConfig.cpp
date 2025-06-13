// 


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputAction(const FGameplayTag& InputTag, bool bLogNotFound) const {

	for (const FAuraInputAction& Action : AbilityInputActions) {
		if (Action.InputAction && Action.InputTag == InputTag) {
			return Action.InputAction;
		}
	}


	if (bLogNotFound) {
		UE_LOG(LogTemp, Error, TEXT("Input %s not found for ability input action"), *InputTag.ToString());
	}

	return nullptr;
}
