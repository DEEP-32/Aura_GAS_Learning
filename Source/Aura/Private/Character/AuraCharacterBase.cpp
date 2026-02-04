#include "Character/AuraCharacterBase.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"

AAuraCharacterBase::AAuraCharacterBase() {
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation() {
	return HitReactMontage;
	
}

void AAuraCharacterBase::Die() {
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));
	MulticastHandleDeath();
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation() {
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
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

void AAuraCharacterBase::Dissolve() {
	if (IsValid(DissolveMaterialInstance)) {
		UMaterialInstanceDynamic* DynamicInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance,this);
		GetMesh()->SetMaterial(0, DynamicInst);
		StartDissolveTimeline(DynamicInst);
	}
	if (IsValid(WeaponDissolveMaterialInstance)) {
		UMaterialInstanceDynamic* DynamicInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance,this);
		Weapon->SetMaterial(0, DynamicInst);
		StartWeaponDissolveTimeline(DynamicInst);
	}
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
