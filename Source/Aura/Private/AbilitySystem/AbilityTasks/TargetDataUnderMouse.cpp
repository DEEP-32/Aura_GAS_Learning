


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility) {
	UTargetDataUnderMouse* TargetData = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	
	
	return TargetData;
}

void UTargetDataUnderMouse::Activate() {
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();

	if (bIsLocallyControlled) {
		SendMouseCursorData();
	}
	else {
		//TODO : We are on the server, so listen for target data.
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(
			SpecHandle,
			ActivationPredictionKey
		).AddUObject(this, &ThisClass::OnTargetReplicatedCallback);

		const bool bCallDelegate  = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle,ActivationPredictionKey);
		if (!bCallDelegate) {
			SetWaitingOnRemotePlayerData();
		}
	}
		
	
}

void UTargetDataUnderMouse::SendMouseCursorData() {

	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());
	
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility,false,HitResult);
	
	Data->HitResult = HitResult;
	DataHandle.Add(Data);

	FGameplayTag ApplicationTag;

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		ApplicationTag,
		AbilitySystemComponent->ScopedPredictionKey
	);

	const bool bShouldBroadcastAbilityTaskDelegates = ShouldBroadcastAbilityTaskDelegates();
	if (bShouldBroadcastAbilityTaskDelegates) {
		TargetData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetReplicatedCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle,
	FGameplayTag ActivationTag) {

	AbilitySystemComponent->ConsumeClientReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey()
	);

	const bool bShouldBroadcastAbilityTaskDelegates = ShouldBroadcastAbilityTaskDelegates();
	if (bShouldBroadcastAbilityTaskDelegates) {
		TargetData.Broadcast(TargetDataHandle);
	}
}

