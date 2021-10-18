// Fill out your copyright notice in the Description page of Project Settings.


#include "VendorCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "HorizonCharacter.h"
#include "HorizonPlayerController.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AVendorCharacter::AVendorCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphere"));
	InteractSphere->SetupAttachment(GetRootComponent());
	InteractSphere->InitSphereRadius(75.f);

}

// Called when the game starts or when spawned
void AVendorCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &AVendorCharacter::InteractSphereOnOverlapBegin);
	InteractSphere->OnComponentEndOverlap.AddDynamic(this, &AVendorCharacter::InteractSphereOnOverlapEnd);


	
}

void AVendorCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	const FString VendorTablePath(TEXT("DataTable'/Game/_Game/Vendor/DataTables/VendorDataTable.VendorDataTable'"));

	UDataTable* VendorTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *VendorTablePath));

	if (VendorTableObject)
	{
		DataTableLoop(VendorTableObject);
	}

}

void AVendorCharacter::OnRep_VendorItems()
{
	if (VendorItems.Num())
	{
		AddItemAndUpdateVendorWidget(VendorItems[VendorItems.Num() - 1], VendorItems);
	}
	else
	{
		AddItemAndUpdateVendorWidget(FItemData(), VendorItems);
	}
}

void AVendorCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AVendorCharacter, VendorItems, COND_OwnerOnly);

}

// Called every frame
void AVendorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVendorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AVendorCharacter::InteractSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
	 Character = Cast<AHorizonCharacter>(OtherActor);
		if (Character) // If true , we have a valid reference to the player.
		{
			bInteracting = true;
			Character->SetInShopRange(true);
			Character->ShowVendorInventory(true);
			Character->ShowInventory(true);
			Character->SetVendorCharacter(this);
			OnRep_VendorItems();
			InitializeInventory();
			DataTableLoop(VendorDataTable);
		}
	}

}
void AVendorCharacter::InteractSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Character = Cast<AHorizonCharacter>(OtherActor);
	if (Character) // If true , we have a valid reference to the player.
	{
		bInteracting = false;
		Character->SetInShopRange(false);
		Character->ShowVendorInventory(false);
		Character->ShowInventory(false);
		Character->SetVendorCharacter(nullptr);
	    VendorItems.Reset();
	}
}

void AVendorCharacter::DataTableLoop(UDataTable* DataTable)
{
	TArray<FName> ItemRowNames;
	ItemRowNames = DataTable->GetRowNames();
	int32 Lenght = ItemRowNames.Num();

	for (int32 i=0; i<Lenght; i++)
	{
		FVendorDataTable* VendorDataRow = nullptr;
		VendorDataRow = DataTable->FindRow<FVendorDataTable>(FName(ItemRowNames[i]), TEXT(""));
		VendorItems.Add(VendorDataRow->ItemData);
		UE_LOG(LogTemp, Warning, TEXT("Vendor Item: %s"), *ItemRowNames[i].ToString());
	}

}






