// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HSCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HSCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "HSComboActionData.h"
#include "Physics/HSCollision.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/HSCharacterStatComponent.h"
#include "UI/HSWidgetComponent.h"
#include "UI/HSHpBarWidget.h"
#include "Item/HSWeaponItemData.h"
#include "Components/SkeletalMeshComponent.h"

DEFINE_LOG_CATEGORY(LogHSCharacter);

// Sets default values
AHSCharacterBase::AHSCharacterBase()
{
 	// Pawn
    bUseControllerRotationRoll = false; // X
    bUseControllerRotationPitch = false; // Y
    bUseControllerRotationYaw = false; // Z

    // Capsule
    GetCapsuleComponent()->InitCapsuleSize(50.f, 100.f);
    GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_HSCAPSULE);

    // Movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f); // Pitch, Yaw, Roll
    GetCharacterMovement()->JumpZVelocity = 500.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    // Mesh
    GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -100.f), FRotator(0.f, -90.f, 0.f));
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Warrior.SK_CharM_Warrior'"));
    if (CharacterMeshRef.Object)
    {
        GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/HackAndSlash/Animation/ABP_HSCharacter.ABP_HSCharacter_C"));
    if (AnimInstanceClassRef.Class)
    {
        GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
    }

    static ConstructorHelpers::FObjectFinder<UHSCharacterControlData> ShoulderDataRef(TEXT("/Script/HackAndSlash.HSCharacterControlData'/Game/HackAndSlash/CharacterControl/HSC_Shoulder.HSC_Shoulder'"));
    if (ShoulderDataRef.Object)
    {
		CharacterControlManager.Emplace(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
    }

    static ConstructorHelpers::FObjectFinder<UHSCharacterControlData> QuaterDataRef(TEXT("/Script/HackAndSlash.HSCharacterControlData'/Game/HackAndSlash/CharacterControl/HSC_Quater.HSC_Quater'"));
    if (QuaterDataRef.Object)
    {
		CharacterControlManager.Emplace(ECharacterControlType::Quater, QuaterDataRef.Object);
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/HackAndSlash/Animation/AM_ComboAttack.AM_ComboAttack'"));
    if (ComboActionMontageRef.Object)
    {
        ComboActionMontage = ComboActionMontageRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UHSComboActionData> ComboActionDataRef(TEXT("/Script/HackAndSlash.HSComboActionData'/Game/HackAndSlash/CharacterAction/HSA_ComboAttack.HSA_ComboAttack'"));
    if (ComboActionDataRef.Object)
    {
        ComboActionData = ComboActionDataRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/HackAndSlash/Animation/AM_Dead.AM_Dead'"));
    if (DeadMontageRef.Object)
    {
        DeadMontage = DeadMontageRef.Object;
    }

    // Stat Component
    Stat = CreateDefaultSubobject<UHSCharacterStatComponent>(TEXT("Stat"));

    // Widget Component
    HpBar = CreateDefaultSubobject<UHSWidgetComponent>(TEXT("Widget"));
    HpBar->SetupAttachment(GetMesh());
    HpBar->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
    static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/HackAndSlash/UI/WBP_HpBar.WBP_HpBar_C"));
    if (HpBarWidgetRef.Class)
    {
        HpBar->SetWidgetClass(HpBarWidgetRef.Class);
        HpBar->SetWidgetSpace(EWidgetSpace::Screen);
        HpBar->SetDrawSize(FVector2D(150.f, 15.f));
        HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    // Item Actions
    TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AHSCharacterBase::EquipWeapon)));
    TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AHSCharacterBase::DrinkPotion)));
    TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &AHSCharacterBase::ReadScroll)));

    Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
    Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
}

void AHSCharacterBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    Stat->OnHpZero.AddUObject(this, &AHSCharacterBase::SetDead);
}

void AHSCharacterBase::SetCharacterControlData(const UHSCharacterControlData* CharacterControlData)
{
    // Pawn
    bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

    // CharacterMovement
    GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
    GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
    GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void AHSCharacterBase::ProcessComboCommand()
{
    if (CurrentCombo == 0)
    {
        ComboActionBegin();
        return;
    }

    if (!ComboTimerHandle.IsValid())
    {
        HasNextComboCommand = false;
    }
    else
    {
        HasNextComboCommand = true;
    }
}

void AHSCharacterBase::ComboActionBegin()
{
    // Combo Status
    CurrentCombo = 1;

    // Movement Setting
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    // Animation Setting
    const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &AHSCharacterBase::ComboActionEnd);
    AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

    ComboTimerHandle.Invalidate();
    SetComboCheckTimer();
}

void AHSCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
    ensure(CurrentCombo != 0);
    CurrentCombo = 0;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

    NotifyComboActionEnd();
}

void AHSCharacterBase::NotifyComboActionEnd()
{
}

void AHSCharacterBase::SetComboCheckTimer()
{
    int32 ComboIndex = CurrentCombo - 1;
    ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

    const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
    float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
    if (ComboEffectiveTime > 0.f)
    {
        GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AHSCharacterBase::ComboCheck, ComboEffectiveTime, false);
    }

}

void AHSCharacterBase::ComboCheck()
{
    ComboTimerHandle.Invalidate();
    if (HasNextComboCommand)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

        CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
        FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
        AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
        SetComboCheckTimer();
        HasNextComboCommand = false;
    }
}

void AHSCharacterBase::AttackHitCheck()
{
    FHitResult OutHitResult;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this); // 콜리전 분석 시 식별자 정보, 복잡한 형태의 충돌체도 감지할 것인지 옵션, 무시할 액터들(자기자신만 무시하면 됨)

    const float AttackRange = Stat->GetTotalStat().AttackRange;
    const float AttackRadius = Stat->GetAttackRadius();
    const float AttackDamage = Stat->GetTotalStat().Attack;
    const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
    const FVector End = Start + GetActorForwardVector() * AttackRange;

    bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_HSACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
    if (HitDetected)
    {
        FDamageEvent DamageEvent;
        OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
    }

#if ENABLE_DRAW_DEBUG

    FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
    float CapsuleHalfHeight = AttackRange * 0.5f + AttackRadius;
    FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

    DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.f);
#endif
}

float AHSCharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    Stat->ApplyDamage(DamageAmount);

    return DamageAmount;
}

void AHSCharacterBase::SetDead()
{
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
    PlayDeadAnimation();
    SetActorEnableCollision(false);
    HpBar->SetHiddenInGame(true);
}

void AHSCharacterBase::PlayDeadAnimation()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
		AnimInstance->StopAllMontages(0.f);
		AnimInstance->Montage_Play(DeadMontage, 1.f);
    }

}

void AHSCharacterBase::SetupCharacterWidget(UHSUserWidget* InUserWidget)
{
    UHSHpBarWidget* HpBarWidget = Cast<UHSHpBarWidget>(InUserWidget);
    if (HpBarWidget)
    {
        HpBarWidget->SetMaxHp(Stat->GetTotalStat().MaxHp);
        HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
        Stat->OnHpChanged.AddUObject(HpBarWidget, &UHSHpBarWidget::UpdateHpBar);
    }
}

void AHSCharacterBase::TakeItem(UHSItemData* InItemData)
{
    if (InItemData)
    {
        TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
    }

}

void AHSCharacterBase::DrinkPotion(UHSItemData* InItemData)
{
    UE_LOG(LogHSCharacter, Log, TEXT("Drink Potion"));
}

void AHSCharacterBase::EquipWeapon(UHSItemData* InItemData)
{
    UHSWeaponItemData* WeaponItemData = Cast<UHSWeaponItemData>(InItemData);

    if (WeaponItemData)
    {
        if (WeaponItemData->WeaponMesh.IsPending())
        {
            WeaponItemData->WeaponMesh.LoadSynchronous();
        }
		Weapon->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());
        Stat->SetModifierStat(WeaponItemData->ModifierStat);
    }
}

void AHSCharacterBase::ReadScroll(UHSItemData* InItemData)
{
    UE_LOG(LogHSCharacter, Log, TEXT("Read Scroll"));
}

int32 AHSCharacterBase::GetLevel()
{
    return Stat->GetCurrentLevel();
}

void AHSCharacterBase::SetLevel(int32 InNewLevel)
{
    Stat->SetLevelStat(InNewLevel);
}