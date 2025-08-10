#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy() {
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	
	
}

void AAuraEnemy::HighlightActor() {
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);	
}


void AAuraEnemy::UnhighlightACtor() {
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AAuraEnemy::GetPlayerLevel() {
	return Level;
}

void AAuraEnemy::BeginPlay() {
	Super::BeginPlay();
	InitAbilityActorInfo();

	UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject());

	UE_LOG(LogTemp,Warning,TEXT("health class : %s called from"),*AuraUserWidget->GetName());

	if (AuraUserWidget) {
		AuraUserWidget->SetWidgetController(this);
	}

	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	if (AuraAttributeSet) {
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).
			AddLambda([this](const FOnAttributeChangeData& OldHealth) {
				OnHealthChanged.Broadcast(OldHealth.NewValue);
			});

		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).
			AddLambda([this](const FOnAttributeChangeData& OldMaxHealth) {
				OnMaxHealthChanged.Broadcast(OldMaxHealth.NewValue);
			});

		OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	}
}

void AAuraEnemy::InitAbilityActorInfo() {
	Super::InitAbilityActorInfo();
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttribute();
}

void AAuraEnemy::InitializeDefaultAttribute() const {
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this,CharacterClass,Level,AbilitySystemComponent);
}
