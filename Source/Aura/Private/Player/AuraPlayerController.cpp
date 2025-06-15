// 


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController() {
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::BeginPlay() {
	Super::BeginPlay();

	check(AuraInputContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem) {
		Subsystem->AddMappingContext(AuraInputContext,0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);

	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);

	AuraInputComponent->BindAbilitiesActions(
		InputConfig,
		this,
		&ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased,
		&ThisClass::AbilityInputTagHeld
	);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue) {
	const FVector2D InputAxiVector = InputActionValue.Get<FVector2D>();
	const FRotator ControllerRotation = GetControlRotation();
	const FRotator YawRotation(0, ControllerRotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>()) {
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxiVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxiVector.X);
	}
	
}

void AAuraPlayerController::CursorTrace() {
	FHitResult CursorHitResult;
	GetHitResultUnderCursor(ECC_Visibility,false,CursorHitResult);
	if (!CursorHitResult.bBlockingHit) return;

	LastHighlightedActor = CurrentHighlightedActor;
	CurrentHighlightedActor = CursorHitResult.GetActor();

	if (LastHighlightedActor == CurrentHighlightedActor)
		return;

	if (LastHighlightedActor != nullptr) {
		LastHighlightedActor->UnhighlightACtor();
	}
	if (CurrentHighlightedActor != nullptr) {
		CurrentHighlightedActor->HighlightActor();
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag) {

	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB)) {
		bTargeting = CurrentHighlightedActor ? true : false;
		bAutoRunning = false;
	}
}


void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag) {

	if (GetASC() == nullptr) return;
	
	GetASC()->AbilityInputTagReleased(InputTag);
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag) {

	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB)) {

		if (GetASC()) {
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}

	if (bTargeting) {
		if (GetASC()) {
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else {
		FollowTime += GetWorld()->GetDeltaSeconds();

		FHitResult HitResult;

		if (GetHitResultUnderCursor(ECC_Visibility,false,HitResult)) {
			CachedDistance = HitResult.ImpactPoint;
		}

		if (APawn* ControllerPawn = GetPawn()) {
			const FVector WorldDirection = (CachedDistance - ControllerPawn->GetActorLocation()).GetSafeNormal();
			ControllerPawn->AddMovementInput(WorldDirection);
		}
	}
	
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC() {
	if (AuraAbilitySystemComponent == nullptr) {
		UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	}

	return AuraAbilitySystemComponent;
}
