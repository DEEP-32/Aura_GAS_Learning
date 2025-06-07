#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase{
	GENERATED_BODY()

public:
	AAuraCharacter();
	
	UPROPERTY(EditAnywhere, Category="Movement")
	float RotationSpeedZ = 540;

	//combat interface
	virtual int32 GetPlayerLevel() override;
	//end combat interface
	
protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	virtual void InitAbilityActorInfo() override;
};
