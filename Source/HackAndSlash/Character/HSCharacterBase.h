// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HSAnimationAttackInterface.h"
#include "Interface/HSCharacterWidgetInterface.h"
#include "Interface/HSCharacterItemInterface.h"

#include "HSCharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHSCharacter, Log, All);

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UHSItemData* /* InItemData */);
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
	FOnTakeItemDelegate ItemDelegate;
};

UCLASS()
class HACKANDSLASH_API AHSCharacterBase : public ACharacter, public IHSAnimationAttackInterface, public IHSCharacterWidgetInterface, public IHSCharacterItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHSCharacterBase();

	virtual void PostInitializeComponents() override;

	// Stat Section
	int32 GetLevel();
	void SetLevel(int32 InNewLevel);

protected:
	virtual void SetCharacterControlData(const class UHSCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UHSCharacterControlData*> CharacterControlManager;

	// Combo Action Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UHSComboActionData> ComboActionData;

	void ProcessComboCommand();

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	virtual void NotifyComboActionEnd();
	void SetComboCheckTimer();
	void ComboCheck();

	int32 CurrentCombo = 0; // 1, 2, 3, 4, 5
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;


	// Attack Hit Section
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	// Dead Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dead, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 5.f;


	// Stat Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UHSCharacterStatComponent> Stat;


	// UI Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UHSWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class UHSUserWidget* InUserWidget) override;


	// Item Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	virtual void TakeItem(class UHSItemData* InItemData) override;
	virtual void DrinkPotion(class UHSItemData* InItemData);
	virtual void EquipWeapon(class UHSItemData* InItemData);
	virtual void ReadScroll(class UHSItemData* InItemData);



};
