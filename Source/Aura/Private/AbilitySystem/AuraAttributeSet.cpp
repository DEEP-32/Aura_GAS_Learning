// 


#include "AuraAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"

UAuraAttributeSet::UAuraAttributeSet() {
	InitHealth(50.f);
	InitMana(50.f);
	InitMaxHealth(100.f);
	InitMaxMana(100.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) {
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute()) {
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute()) {
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxMana());
	}
	
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
	Super::PostGameplayEffectExecute(Data);
	FGameplayEffectContextHandle GameplayEffectContextHandle = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* SourceAsc = GameplayEffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(SourceAsc) && SourceAsc->AbilityActorInfo.IsValid() && SourceAsc->AbilityActorInfo->AvatarActor.IsValid()) {
		AActor* SourceActor = SourceAsc->AbilityActorInfo->AvatarActor.Get();
		const AController* SourceController = SourceAsc->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr) {
			if (const APawn* SourcePawn = Cast<APawn>(SourceActor)) {
				SourceController = SourcePawn->GetController();	
			}
		}
		if (SourceController) {
			ACharacter* SourceCharacter = Cast<ACharacter>(SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo->AvatarActor.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid()) {
		AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		AController* TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		ACharacter* TargetCharacter = Cast<ACharacter>(TargetActor);
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const {

	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Health,OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Health,OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Health,OldMana);
}
void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Health,OldMaxMana);
}
