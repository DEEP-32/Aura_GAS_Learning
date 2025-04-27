#include "Character/AuraEnemy.h"

void AAuraEnemy::HighlightActor() {
	UE_LOG(LogTemp, Warning, TEXT("Enemy Highlighted"));
	bHighlighted = true;
}


void AAuraEnemy::UnhighlightACtor() {
	UE_LOG(LogTemp, Warning, TEXT("Enemy Unhighlighted"));
	bHighlighted = false;
}
