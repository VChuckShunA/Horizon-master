// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Item.h"
#include "Engine/DataTable.h"
#include "VendorCharacter.generated.h"




USTRUCT(BlueprintType)
struct FVendorDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FItemData ItemData;

	EItemState ItemState;

};




UCLASS()
class HORIZON_API AVendorCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AVendorCharacter();

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* InteractSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Properties", meta = (AllowPrivateAccess = "true"))
		bool bInteracting;

	UPROPERTY(ReplicatedUsing = OnRep_VendorItems, BlueprintReadWrite, Category = "Inventory")
		TArray<FItemData> VendorItems;

	int32 InventoryCapacity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
	UDataTable* VendorDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* VInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		AHorizonCharacter* Character = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Datatable")
		FString VendorTablePathX;


	class AHorizonPlayerController* PlayerController;

private:
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void OnRep_VendorItems();

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void AddItemAndUpdateVendorWidget(FItemData ItemData, const TArray<FItemData>& CurrentInventory = TArray<FItemData>());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void InteractSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void InteractSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void AddInventoryItem(FItemData ItemData);

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeInventory();

	void SetVInventory(UUserWidget* Inventory) { VInventory = Inventory; }

	TArray<FItemData> GetInventoryItems() const { return VendorItems; }

	void DataTableLoop(UDataTable* DataTable);
};

