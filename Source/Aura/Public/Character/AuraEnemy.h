#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

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

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Character class defaults")
	int32 Level = 1;
};
