// 


#include "AbilitySystem/Data/CharacterClassInfo.h"
FCharacterClassDefaultInfo UCharacterClassInfo::GetCharacterClassDefaultInfo(ECharacterClass CharacterClass) {
	return CharacterClassInfo.FindChecked(CharacterClass);
}
