// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HSCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AHSCharacterPlayer::AHSCharacterPlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // 소켓 이름을 지정해주면 스프링 암 끝에 자동으로 부착됨
	FollowCamera->bUsePawnControlRotation = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/HackAndSlash/Input/IMC_Default.IMC_Default'"));
	if (InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputJumpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HackAndSlash/Input/Actions/IA_Jump.IA_Jump'"));
	if (InputJumpActionRef.Object)
	{
		JumpAction = InputJumpActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputMoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HackAndSlash/Input/Actions/IA_Move.IA_Move'"));
	if (InputMoveActionRef.Object)
	{
		MoveAction = InputMoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputLookActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/HackAndSlash/Input/Actions/IA_Look.IA_Look'"));
	if (InputLookActionRef.Object)
	{
		LookAction = InputLookActionRef.Object;
	}
}

void AHSCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHSCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHSCharacterPlayer::Look);
}

void AHSCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AHSCharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AHSCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// 입력값으로 컨트롤러의 Control Rotation 속성 업데이트
	AddControllerYawInput(LookAxisVector.X); 
	AddControllerPitchInput(LookAxisVector.Y);
}
