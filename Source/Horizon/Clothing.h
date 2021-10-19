// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Clothing.generated.h"

/**
 * 
 */
class AHorizonCharacter;

UENUM(BlueprintType)
enum class EClothingType : uint8
{
	ECT_Top UMETA(DisplayName= "Shirt"),
	ECT_Bottom UMETA(DisplayName = "Pants"),
	ECT_Glasses UMETA(DisplayName = "Glasses"),

	ECT_MAX UMETA(DisplayName="DefaultMAX")

};
UCLASS()//Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class HORIZON_API AClothing : public AItem
{
	GENERATED_BODY()

	
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent* ClothingMesh;
	
	AClothing();
	virtual void Use(AHorizonCharacter* Character) override;

	//Text for equipping the clothing/changing body part
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Clothing")
	FText UseActionText;

	//Item Mesh picking it off the ground
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Clothing")
		class UStaticMesh* PickupMesh;


	//Item Thumbnail
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Clothing")
		class UTexture2D* Thumbnail;


	//Item Display Name
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Clothing")
	FText ItemDisplayName;


	//Item description
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Clothing",meta=(MultiLine=true))
	FText ItemDescription;

	//Just incase we need a weight limit for the inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Clothing", meta = (ClampMin = 0.0))
	float Weight;

	//UPROPERTY()
	//class UInventoryComponent* OwningInventory;
};
