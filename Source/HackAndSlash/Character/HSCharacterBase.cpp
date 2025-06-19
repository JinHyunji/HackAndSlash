// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HSCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AHSCharacterBase::AHSCharacterBase()
{
 	// Pawn
    bUseControllerRotationRoll = false; // X
    bUseControllerRotationPitch = false; // Y
    bUseControllerRotationYaw = false; // Z

    // Capsule
    GetCapsuleComponent()->InitCapsuleSize(25.f, 100.f);
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

    // Movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f); // Pitch, Yaw, Roll
    GetCharacterMovement()->JumpZVelocity = 500.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    // Mesh
    GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 10.f, -100.f), FRotator(0.f, -90.f, 0.f));
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef = TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'");
    if (CharacterMeshRef.Object)
    {
        GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef = TEXT("/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C");
    if (AnimInstanceClassRef.Class)
    {
        GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
    }
}