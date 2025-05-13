// 


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


AAuraEffectActor::AAuraEffectActor() {
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay() {
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass) {
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) {
		return;
	}

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(
		GameplayEffectClass, ActorLevel, EffectContextHandle
	);

	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
	const bool bEffectCanBeRemoved = InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap;
	if (bIsInfinite && bEffectCanBeRemoved) {
		ActiveEffectHandles.Add(ActiveEffectHandle,TargetASC);
		UE_LOG(LogTemp,Warning,TEXT("AFFECT ACTOR : Added Active Effect Handle , total effects : %d"),ActiveEffectHandles.Num());	
	}
	
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor) {
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		ApplyEffectToTarget(TargetActor,InstantEffect);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		ApplyEffectToTarget(TargetActor,DurationEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		ApplyEffectToTarget(TargetActor,InfiniteEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor) {
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
		ApplyEffectToTarget(TargetActor,InstantEffect);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
		ApplyEffectToTarget(TargetActor,DurationEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap) {
		ApplyEffectToTarget(TargetActor,InfiniteEffectClass);
	}

	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap) {
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		const FActiveGameplayEffectHandle* ActiveGameplayEffectHandle = ActiveEffectHandles.FindKey(TargetASC);
		TargetASC->RemoveActiveGameplayEffect(*ActiveGameplayEffectHandle,1);
		ActiveEffectHandles.Remove(*ActiveGameplayEffectHandle);
		UE_LOG(LogTemp,Warning,TEXT("AFFECT ACTOR : Removed Active Effect Handle , total effects : %d"),ActiveEffectHandles.Num());
	}
}
