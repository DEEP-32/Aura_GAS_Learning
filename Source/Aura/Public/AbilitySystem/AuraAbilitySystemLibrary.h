// 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "AuraAbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|WidgetController",meta=(WorldContext="WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|WidgetController",meta=(WorldContext="WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|CharacterClassDefaults",meta=(WorldContext="WorldContextObject"))
	static void InitializeDefaultAttributes(const UObject* WorldContextObject,ECharacterClass CharacterClass,float Level,UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|CharacterClassDefaults",meta=(WorldContext="WorldContextObject"))
	static void InitializeStartupAbilities(const UObject* WorldContextObject,ECharacterClass CharacterClass,UAbilitySystemComponent* Asc);
	
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|Gameplay Effects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|Gameplay Effects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|Gameplay Effects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,bool bisBlockedHit);
	
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|Gameplay Effects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle,bool bisCriticalHit);
};
