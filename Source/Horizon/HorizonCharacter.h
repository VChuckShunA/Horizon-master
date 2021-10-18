// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Item.h"
#include "HorizonCharacter.generated.h"


UCLASS(config=Game)
class AHorizonCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AHorizonCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AHorizonPlayerController* HorizonPlayerController;


	/* Character Properties*/

	UPROPERTY(ReplicatedUsing = OnRep_Currency, BlueprintReadOnly,Category = "Currency")
	int32 Coins;

	UPROPERTY(ReplicatedUsing = OnRep_Currency, BlueprintReadOnly, Category = "Currency")
	int32 Diamonds;


	UPROPERTY(EditAnywhere,BlueprintReadOnly ,Category = "Properties")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Properties")
	float Health;


	/* Variables for the Shop functionality*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	bool bInShopRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	class AVendorCharacter* VendorCharacter;

	/* Variables for Swiming*/


	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Swimming") 
	bool IsInWater;

	/* When Overlapping with the water , this value is set,using setter function , from the Water Class*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Swimming")
	float InitialWaterHeight;
	

	//Clothing System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	USkeletalMeshComponent* TopMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	USkeletalMeshComponent* BottomMesh;
	
private:

	

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(ReplicatedUsing= OnRep_InventoryItems, BlueprintReadWrite,Category = "Inventory")
	TArray<FItemData> InventoryItems;



	UFUNCTION()
	void OnRep_Currency();


	UFUNCTION()
	void OnRep_InventoryItems();



	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void AddItemAndUpdateInventoryWidget(FItemData ItemData,const TArray<FItemData>& CurrentInventory = TArray<FItemData>());

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItem(TSubclassOf<AItem>ItemSubclass);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUseItem(TSubclassOf<AItem>ItemSubclass);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void BuyItem(FItemData ItemData);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBuyItem(FItemData ItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SellItem(FItemData ItemData);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSellItem(FItemData ItemData);




	UFUNCTION(BlueprintNativeEvent, Category = "Inventory")
	void UpdateCurrency(int32 InventoryCoins);

	void Interact();
	void Interact(FVector Start, FVector End);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Interact(FVector Start, FVector End);


	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void ToggleInventory();

	




protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE void SetInShopRange(bool InRange) { bInShopRange = InRange; }

	void AddInventoryItem(FItemData ItemData);

	void AddCoins(int32 Value);



	void AddDiamonds(int32 Value);

	void AddHealth(float Value);

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void ShowVendorInventory(bool Visible);

	void ShowInventory(bool Visible);

	void SetVendorCharacter(AVendorCharacter* Vendor) { VendorCharacter = Vendor; }
	AVendorCharacter* GetVendorCharacter() const { return VendorCharacter; }

	/* Functions to Handle Swimming*/
	UFUNCTION(BlueprintCallable)
		bool GetIsInWater() const { return IsInWater; }

	UFUNCTION(BlueprintCallable)
		void SetIsInWater(bool InWater) { IsInWater = InWater; }

	UFUNCTION(BlueprintCallable)
		void SetInitialWaterHeight(float WaterHeight) { InitialWaterHeight = WaterHeight; }

	UFUNCTION(BlueprintCallable)
		void StartSwimming();

	UFUNCTION(BlueprintCallable)
		void StopSwimming();



};




