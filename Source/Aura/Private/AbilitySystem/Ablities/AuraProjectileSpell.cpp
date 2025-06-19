// 


#include "AbilitySystem/Ablities/AuraProjectileSpell.h"

#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData) {
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UKismetSystemLibrary::PrintString(
		this,
		FString("Activating Ability (C++)"),
		true,
		true,
		FLinearColor::Yellow,
		5.f
	);
}
