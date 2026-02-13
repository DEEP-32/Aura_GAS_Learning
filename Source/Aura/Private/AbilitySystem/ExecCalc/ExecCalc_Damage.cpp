


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics {
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	
	
	AuraDamageStatics() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false);	
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target,false);	
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration,Source,false)
	}
};

static const AuraDamageStatics DamageStatics() {
	static AuraDamageStatics Statics;
	return Statics;
}


UExecCalc_Damage::UExecCalc_Damage() {
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
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
	
	float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
	
	
	
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
	
	const float EffectiveArmor = TargetArmor * (100  - SourceArmorPenetration * ArmonPenetrationCoff) / 100.f; 
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoff) / 100.f;
	
	const FGameplayModifierEvaluatedData EvaluateData(UAuraAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluateData);
}
