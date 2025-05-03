// 


#include "UI/WidgetController/AuraWidgetController.h"
void UAuraWidgetController::Initialize(const FWidgetControlledParams& Params) {
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues() {
	
}
