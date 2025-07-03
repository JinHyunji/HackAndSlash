// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/HSStageGimmick.h"
#include "Components/BoxComponent.h"
#include "Physics/HSCollision.h"
#include "Character/HSCharacterNonPlayer.h"
#include "Engine/OverlapResult.h"
#include "Item/HSItemBox.h"

// Sets default values
AHSStageGimmick::AHSStageGimmick()
{
	// Stage Section
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = Stage;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/HackAndSlash/Environment/Stages/SM_SQUARE.SM_SQUARE'"));
	if (StageMeshRef.Object)
	{
		Stage->SetStaticMesh(StageMeshRef.Object);
	}

	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTrigger->SetBoxExtent(FVector(775.f, 775.f, 300.f));
	StageTrigger->SetupAttachment(Stage);
	StageTrigger->SetRelativeLocation(FVector(0.f, 0.f, 250.f));
	StageTrigger->SetCollisionProfileName(CPROFILE_HSTRIGGER);
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AHSStageGimmick::OnStageTriggerBeginOverlap);

	// Gate Section
	static FName GateSockets[] = { TEXT("+XGate"), TEXT("-XGate"), TEXT("+YGate"), TEXT("-YGate") };
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/HackAndSlash/Environment/Props/SM_GATE.SM_GATE'"));
	if (GateMeshRef.Object)
	{
		for (FName GateSocket : GateSockets)
		{
			UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);
			Gate->SetStaticMesh(GateMeshRef.Object);
			Gate->SetupAttachment(Stage, GateSocket);
			Gate->SetRelativeLocationAndRotation(FVector(0.f, -80.5f, 0.f), FRotator(0.f, -90.f, 0.f));
			Gates.Add(GateSocket, Gate);

			FName TriggerName = *GateSocket.ToString().Append(TEXT("Trigger"));
			UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);
			GateTrigger->SetBoxExtent(FVector(100.f, 100.f, 300.f));
			GateTrigger->SetupAttachment(Stage, GateSocket);
			GateTrigger->SetRelativeLocation(FVector(70.f, 0.f, 250.f));
			GateTrigger->SetCollisionProfileName(CPROFILE_HSTRIGGER);
			GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AHSStageGimmick::OnGateTriggerBeginOverlap);
			GateTrigger->ComponentTags.Add(GateSocket);

			GateTriggers.Add(GateTrigger);
		}
	}

	// State Section
	CurrentState = EStageState::READY;
	StageChangeActions.Add(EStageState::READY, FStageChangedDelegateWrapper(FOnStageChangeDelegate::CreateUObject(this, &AHSStageGimmick::SetReady)));
	StageChangeActions.Add(EStageState::FIGHT, FStageChangedDelegateWrapper(FOnStageChangeDelegate::CreateUObject(this, &AHSStageGimmick::SetFight)));
	StageChangeActions.Add(EStageState::REWARD, FStageChangedDelegateWrapper(FOnStageChangeDelegate::CreateUObject(this, &AHSStageGimmick::SetChooseReward)));
	StageChangeActions.Add(EStageState::NEXT, FStageChangedDelegateWrapper(FOnStageChangeDelegate::CreateUObject(this, &AHSStageGimmick::SetChooseNext)));

	// Fight Section
	OpponentSpawnTime = 2.f;
	OpponentClass = AHSCharacterNonPlayer::StaticClass();

	// Reward Section
	RewardBoxClass = AHSItemBox::StaticClass();
	for (FName GateSocket : GateSockets)
	{
		FVector BoxLocation = Stage->GetSocketLocation(GateSocket) / 2;
		RewardBoxLocations.Add(GateSocket, BoxLocation);
	}

	// Stat Section
	CurrentStageNum = 0;
}

void AHSStageGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetState(CurrentState);
}

void AHSStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	SetState(EStageState::FIGHT);

}

void AHSStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	check(OverlappedComponent->ComponentTags.Num() == 1);
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	check(Stage->DoesSocketExist(SocketName));

	FVector NewLocation = Stage->GetSocketLocation(SocketName);
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(GateTrigger), false, this); // 자신은 제외하고 콜리전 검사
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams::InitType::AllStaticObjects,
		FCollisionShape::MakeSphere(775.f),
		CollisionQueryParam
	);

	if (!bResult)
	{
		FTransform NewTransform(NewLocation);
		AHSStageGimmick* NewGimmick = GetWorld()->SpawnActorDeferred<AHSStageGimmick>(AHSStageGimmick::StaticClass(), NewTransform);
		if (NewGimmick)
		{
			NewGimmick->SetStageNum(CurrentStageNum + 1);
			NewGimmick->FinishSpawning(NewTransform);
		}
	}
}

void AHSStageGimmick::OpenAllGates()
{
	for (auto Gate : Gates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}
}

void AHSStageGimmick::CloseAllGates()
{
	for (auto Gate : Gates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator::ZeroRotator);
	}
}

void AHSStageGimmick::SetState(EStageState InNewState)
{
	CurrentState = InNewState;

	if (StageChangeActions.Contains(CurrentState))
	{
		StageChangeActions[CurrentState].StageDelegate.ExecuteIfBound();
	}
}

void AHSStageGimmick::SetReady()
{
	StageTrigger->SetCollisionProfileName(CPROFILE_HSTRIGGER);
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	OpenAllGates();
}

void AHSStageGimmick::SetFight()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	CloseAllGates();

	GetWorld()->GetTimerManager().SetTimer(OpponentTimerHandle, this, &AHSStageGimmick::OnOpponentSpawn, OpponentSpawnTime, false);
}

void AHSStageGimmick::SetChooseReward()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}

	CloseAllGates();
	SpawnRewardBoxes();
}

void AHSStageGimmick::SetChooseNext()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(CPROFILE_HSTRIGGER);
	}

	OpenAllGates();
}

void AHSStageGimmick::OnOpponentDestroyed(AActor* DestroyedActor)
{
	SetState(EStageState::REWARD);
}

void AHSStageGimmick::OnOpponentSpawn()
{
	const FTransform SpawnTransform(GetActorLocation() + FVector::UpVector * 88.f);
	AHSCharacterNonPlayer* HSOpponentCharacter = GetWorld()->SpawnActorDeferred<AHSCharacterNonPlayer>(OpponentClass, SpawnTransform);
	if (HSOpponentCharacter)
	{
		HSOpponentCharacter->OnDestroyed.AddDynamic(this, &AHSStageGimmick::OnOpponentDestroyed);
		HSOpponentCharacter->SetLevel(CurrentStageNum);
		HSOpponentCharacter->FinishSpawning(SpawnTransform);
	}
}

void AHSStageGimmick::OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	for (const auto& RewardBox : RewardBoxes)
	{
		if (RewardBox.IsValid())
		{
			AHSItemBox* ValidItemBox = RewardBox.Get();
			AActor* OverlappedBox = OverlappedComponent->GetOwner();
			if (OverlappedBox != ValidItemBox) // 선택되지 않은 나머지 상자 모두 제거
			{
				ValidItemBox->Destroy();
			}
		}
	}

	SetState(EStageState::NEXT);
}

void AHSStageGimmick::SpawnRewardBoxes()
{
	for (const auto& RewardBoxLocation : RewardBoxLocations)
	{
		FTransform SpawnTransform(GetActorLocation() + RewardBoxLocation.Value + FVector(0.f, 0.f, 30.f));
		AHSItemBox* RewardBoxActor = GetWorld()->SpawnActorDeferred<AHSItemBox>(RewardBoxClass, SpawnTransform);
		if (RewardBoxActor)
		{
			RewardBoxActor->Tags.Add(RewardBoxLocation.Key);
			RewardBoxActor->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &AHSStageGimmick::OnRewardTriggerBeginOverlap);
			RewardBoxes.Add(RewardBoxActor);
		}
	}

	for (const auto& RewardBox : RewardBoxes)
	{
		if (RewardBox.IsValid())
		{
			RewardBox.Get()->FinishSpawning(RewardBox.Get()->GetActorTransform());
		}
	}
}
