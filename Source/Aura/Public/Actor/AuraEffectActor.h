// 

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
class USphereComponent;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8 {
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM()
enum class EEffectRemovalPolicy {
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor {
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	bool bDestroyOnEffectRemoval = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	TSubclassOf<UGameplayEffect> InstantEffect;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	TSubclassOf<UGameplayEffect> DurationEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	TSubclassOf<UGameplayEffect> InfiniteEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="Effects")
	float ActorLevel = 1.f;
};
