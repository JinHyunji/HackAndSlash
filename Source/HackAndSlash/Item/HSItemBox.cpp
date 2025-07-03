// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/HSItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Physics/HSCollision.h"
#include "Interface/HSCharacterItemInterface.h"
#include "Engine/AssetManager.h"
#include "HSItemData.h"

// Sets default values
AHSItemBox::AHSItemBox()
{
    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));

    RootComponent = Trigger;
    Mesh->SetupAttachment(Trigger);
    Effect->SetupAttachment(Trigger);

    Trigger->SetCollisionProfileName(CPROFILE_HSTRIGGER);
    Trigger->SetBoxExtent(FVector(40.f, 42.f, 30.f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/HackAndSlash/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
    if (BoxMeshRef.Object)
    {
        Mesh->SetStaticMesh(BoxMeshRef.Object);
    }
    Mesh->SetRelativeLocation(FVector(0.f, -3.5f, -30.f));
    Mesh->SetCollisionProfileName(TEXT("NoCollision"));

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> EffectRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/sA_PickupSet_1/Fx/NiagaraSystems/NS_CoinBurst.NS_CoinBurst'"));
    if (EffectRef.Object)
    {
        Effect->SetAsset(EffectRef.Object);
        Effect->bAutoActivate = false;
    }
}

void AHSItemBox::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    UAssetManager& Manager = UAssetManager::Get();

    TArray<FPrimaryAssetId> Assets;
    Manager.GetPrimaryAssetIdList(TEXT("HSItemData"), Assets);
    ensure(0 < Assets.Num());

    int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
    FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
    if (AssetPtr.IsPending())
    {
        AssetPtr.LoadSynchronous();
    }
    Item = Cast<UHSItemData>(AssetPtr.Get());
    ensure(Item);

    Trigger->OnComponentBeginOverlap.AddDynamic(this, &AHSItemBox::OnOverlapBegin);
}

void AHSItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
    if (!Item)
    {
        Destroy();
        return;
    }

    IHSCharacterItemInterface* OverlappingPawn = Cast<IHSCharacterItemInterface>(OtherActor);
    if (OverlappingPawn)
    {
        OverlappingPawn->TakeItem(Item);
    }

    if (Effect && Mesh)
    {
		Effect->SetAutoDestroy(true);
        Effect->ActivateSystem(true);
		Mesh->SetHiddenInGame(true);
		SetActorEnableCollision(false);
		Effect->OnSystemFinished.AddDynamic(this, &AHSItemBox::OnEffectFinished);
    }
}

void AHSItemBox::OnEffectFinished(UNiagaraComponent* ParticleSystem)
{
    Destroy();
}

