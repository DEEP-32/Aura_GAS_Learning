#pragma once

#include  "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext {
	GENERATED_BODY()

public:
	bool IsCriticalHit() const { return bIsCriticalHit; }
	void SetIsCriticalHit(const bool IsCriticalHit) { this->bIsCriticalHit = IsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }
	void SetIsBlockedHit(const bool IsBlockedHit) { this->bIsBlockedHit = IsBlockedHit; }

	virtual UScriptStruct* GetScriptStruct() const {
		return StaticStruct();
	}

	virtual FAuraGameplayEffectContext* Duplicate() const {
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult()) {
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:
	UPROPERTY()
	bool bIsCriticalHit = false;
	UPROPERTY()
	bool bIsBlockedHit = false;
};

template <>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext> {
	enum {
		WithNetSerializer = true,
		WithCopy = true
	};
};
