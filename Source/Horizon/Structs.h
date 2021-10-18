#pragma once

#include "Structs.generated.h"

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Pickup UMETA(DisplayName = "Pickup"),
	EIS_InInventory UMETA(DisplayName = "In Inventory"),
	EIS_ForSale UMETA(DisplayName = "ForSale"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_MAX UMETA(DisplayName = "DefaultMax")

};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class AItem> ItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UTexture2D* ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 ItemCost;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		bool IsStackable;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		int32 StackCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemState ItemState;
};