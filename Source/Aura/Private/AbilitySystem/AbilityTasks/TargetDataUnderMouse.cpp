


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility) {
	UTargetDataUnderMouse* TargetData = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	
	
	return TargetData;
}

void UTargetDataUnderMouse::Activate() {
	Super::Activate();

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility,false,HitResult);

	TargetPosition.Broadcast(HitResult.Location);
}
