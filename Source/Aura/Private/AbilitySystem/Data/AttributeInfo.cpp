


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoFromTag(FGameplayTag AttributeTag, bool bLogNotFound) const {
	for (const FAuraAttributeInfo& Info : AttributesInfo) {
		if (Info.AttributeTag.MatchesTagExact(AttributeTag)) {
			return Info;
		}
	}

	if (bLogNotFound) {
		UE_LOG(LogTemp, Warning, TEXT("Attribute %s not found"), *AttributeTag.ToString());
	}

	return FAuraAttributeInfo();
}
