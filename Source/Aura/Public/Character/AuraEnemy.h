#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface{
	GENERATED_BODY()

public:
	AAuraEnemy();

	//Enemy interface
	virtual void HighlightActor() override;
	virtual void UnhighlightACtor() override;
	//End Enemy interface

	//Combat Interface
	virtual int32 GetPlayerLevel() override;
	
	//End Combat Interface
	
	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	//Callbacks
	void HitRectTagChanged(const FGameplayTag CallbackTag,int32 NewCount);

	UPROPERTY(BlueprintReadOnly,Category="Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	float BaseWalkSpeed;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttribute() const override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Character class defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Character class defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="UI")
	TObjectPtr<UWidgetComponent> HealthBar;

	
};
