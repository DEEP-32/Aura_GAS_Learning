#include "Character/AuraCharacterBase.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"

AAuraCharacterBase::AAuraCharacterBase() {
	PrimaryActorTick.bCanEverTick = false;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAuraCharacterBase::BeginPlay() {
	Super::BeginPlay();
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

FVector AAuraCharacterBase::GetCombatSocketLocation() {
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AAuraCharacterBase::InitAbilityActorInfo() {
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const {
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
		GameplayEffectClass, Level, EffectContextHandle
	);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void AAuraCharacterBase::AddCharacterAbilities() {

	if (!HasAuthority()) return;

	UAuraAbilitySystemComponent* AuraAsc = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	check(AuraAsc);
	AuraAsc->AddCharacterAbilities(AbilityClasses);
}

void AAuraCharacterBase::InitializeDefaultAttribute() const {
	ApplyEffectToSelf(DefaultPrimaryAttributes,1);
	ApplyEffectToSelf(DefaultSecondaryAttributes,1);
	ApplyEffectToSelf(DefaultVitalAttributes,1);
}
