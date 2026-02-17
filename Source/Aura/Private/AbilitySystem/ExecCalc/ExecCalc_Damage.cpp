


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics {
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	
	
	TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDef;
	
	
	AuraDamageStatics() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false);	
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target,false);	
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitChance,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitDamage,Source,false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,FireResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArcaneResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,LightningResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,PhysicalResistance,Target,false);
		
		TagsToCaptureDef.Add(
			FAuraGameplayTags::Get().Attributes_Secondary_Armor,
			ArmorDef	
		);
		TagsToCaptureDef.Add(
			FAuraGameplayTags::Get().Attributes_Secondary_BlockChance,
			BlockChanceDef
		);
		TagsToCaptureDef.Add(
			FAuraGameplayTags::Get().Attributes_Secondary_ArmorPenetration,
			ArmorPenetrationDef	
		);
		TagsToCaptureDef.Add(
			FAuraGameplayTags::Get().Attributes_Secondary_CriticalHitChance,
			CriticalHitChanceDef
		);
		TagsToCaptureDef.Add(
			FAuraGameplayTags::Get().Attributes_Secondary_CriticalHitResistance,
			CriticalHitResistanceDef
		);
		TagsToCaptureDef.Add(
			FAuraGameplayTags::Get().Attributes_Secondary_CriticalHitDamage,
			CriticalHitDamageDef
		);
		TagsToCaptureDef.Add(
			FAuraGameplayTags::Get().Attribute_Resistance_Fire,
			FireResistanceDef	
		);
		TagsToCaptureDef.Add(
			FAuraGameplayTags::Get().Attribute_Resistance_Lightning,
			LightningResistanceDef	
		);
		TagsToCaptureDef.Add(
			FAuraGameplayTags::Get().Attribute_Resistance_Arcane,
			ArcaneResistanceDef
		);
		TagsToCaptureDef.Add(
			FAuraGameplayTags::Get().Attribute_Resistance_Physical,
			PhysicalResistanceDef	
		);
		
	}
};

static const AuraDamageStatics& DamageStatics() {
	static AuraDamageStatics Statics;
	return Statics;
}


UExecCalc_Damage::UExecCalc_Damage() {
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	
	const UAbilitySystemComponent* SourceAsc = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAst = ExecutionParams.GetTargetAbilitySystemComponent();
	
	AActor* SourceAvatar = SourceAsc ? SourceAsc->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetAst ? TargetAst->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCombat = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombat = Cast<ICombatInterface>(TargetAvatar);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();
	
	EvaluateParameters.SourceTags = SourceTag;
	EvaluateParameters.TargetTags = TargetTag;
	
	
	
	float Damage = 0;
	FGameplayTagContainer AllDamageTags = UGameplayTagsManager::Get().RequestGameplayTagChildren(FAuraGameplayTags::Get().Damage);
	for (const auto& PairTag : FAuraGameplayTags::Get().DamageTypesToResistance ) {
		const FGameplayEffectAttributeCaptureDefinition ResistanceDef = AuraDamageStatics().TagsToCaptureDef[PairTag.Value];
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ResistanceDef,EvaluateParameters,Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f,100.f);
		float DamageValueType = Spec.GetSetByCallerMagnitude(PairTag.Key);
		DamageValueType *= (100.f - Resistance) / 100.f;
		Damage += DamageValueType;
	}
	
	
	
	//capture block chance
	//if block , halve the damage
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,EvaluateParameters,TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);
	
	const bool bIsBlocked = FMath::RandRange(0.f,100.f) < TargetBlockChance;
	Damage *= bIsBlocked ? 0.5f : 1.f;
	
	//Armor penetration ignores a percentage of the target's Armor.
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParameters,TargetArmor);
	TargetBlockChance = FMath::Max<float>(TargetArmor, 0.f);
	
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,EvaluateParameters,SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);
	
	
	UCharacterClassInfo* CharacterInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const float ArmonPenetrationCoff = CharacterInfo->ArmorPenetrationModifierCoeffecient.GetValueAtLevel(SourceCombat->GetPlayerLevel());
	const float EffectiveArmorCoff = CharacterInfo->EffectiveModifierCoeffecient.GetValueAtLevel(TargetCombat->GetPlayerLevel());
	
	const float EffectiveArmor = TargetArmor * (100.f  - SourceArmorPenetration * ArmonPenetrationCoff) / 100.f; 
	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoff) / 100.f;
	
	
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,EvaluateParameters,SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);
	
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef,EvaluateParameters,TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);
	
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef,EvaluateParameters,SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);
	
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * 0.15f;
	
	const bool bIsCriticalHit = FMath::RandRange(0.f,100.f) < EffectiveCriticalHitChance;
	Damage = bIsCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;
	
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle,bIsBlocked);
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle,bIsCriticalHit);
	
	
	const FGameplayModifierEvaluatedData EvaluateData(UAuraAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluateData);
}
