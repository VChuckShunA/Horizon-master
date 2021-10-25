// Copyright Epic Games, Inc. All Rights Reserved.

#include "HorizonCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "HorizonPlayerController.h"
#include "Item.h"
#include "GameFramework/PhysicsVolume.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

AHorizonCharacter::AHorizonCharacter() :
MaxHealth(100.f),
Health (50.f),
bInShopRange(false),
Coins(100)

{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Initialize Clothing System
	TopMesh=CreateDefaultSubobject<USkeletalMeshComponent>(FName("TopMesh"));
	TopMesh->SetupAttachment(GetMesh());
	BottomMesh=CreateDefaultSubobject<USkeletalMeshComponent>(FName("BottomMesh"));
	BottomMesh->SetupAttachment(GetMesh());
	
	//Initialize Hunger/Thirst system
	Food = 100.f;
	Water = 100.f;
	MaxFood = 100.f;
	MaxWater = 100.f;
	FoodWaterDrainRate = 2.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AHorizonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AHorizonCharacter::ToggleInventory);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AHorizonCharacter::Interact);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHorizonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHorizonCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AHorizonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AHorizonCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AHorizonCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AHorizonCharacter::TouchStopped);

}





void AHorizonCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AHorizonCharacter, InventoryItems, COND_OwnerOnly);
	DOREPLIFETIME(AHorizonCharacter, Coins);
	DOREPLIFETIME(AHorizonCharacter, Diamonds);
}

void AHorizonCharacter::OnRep_Currency()
{
	if (IsLocallyControlled())
	{
		UpdateCurrency(Coins);
	}
	
}

void AHorizonCharacter::Interact()
{
	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + FollowCamera->GetForwardVector() * 500.f;

	if (HasAuthority())
	{
		Interact(Start, End);
	}
	else
	{
		Server_Interact(Start, End);
	}
	

}

void AHorizonCharacter::Interact(FVector Start, FVector End)
{


	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // Ignore our player

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
	{
		if (IInteractInterface* Interface = Cast<IInteractInterface>(HitResult.GetActor()))
		{
			Interface->Interact(this);
		}
	}
}

bool AHorizonCharacter::Server_Interact_Validate(FVector Start, FVector End)
{
	return true;
}

void AHorizonCharacter::Server_Interact_Implementation(FVector Start, FVector End)
{
	Interact(Start, End);
}



void AHorizonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AHorizonCharacter::AddCoins(int32 Value)
{
	Coins += Value;
	if (IsLocallyControlled())
	{
		UpdateCurrency(Coins);
	}
	UE_LOG(LogTemp, Warning, TEXT("Added Coins: %d"), Coins);
}

void AHorizonCharacter::BuyItem(FItemData ItemData)
{
	if (ItemData.ItemCost > Coins) return;

	Coins -= ItemData.ItemCost;
	AddInventoryItem(ItemData);
	if (IsLocallyControlled())
	{
		UpdateCurrency(Coins);
		OnRep_InventoryItems();
	}

}

void AHorizonCharacter::ServerBuyItem_Implementation(FItemData ItemData)
{
}

bool AHorizonCharacter::ServerBuyItem_Validate(FItemData ItemData)
{
	return true;
}
void AHorizonCharacter::ServerSellItem_Implementation(FItemData ItemData)
{
	for (FItemData& Item : InventoryItems)
	{
		if (Item.ItemClass == ItemData.ItemClass)
		{
			SellItem(ItemData);
			return;
		}
	}
}

bool AHorizonCharacter::ServerSellItem_Validate(FItemData ItemData)
{
	return true;
} 


void AHorizonCharacter::SellItem(FItemData ItemData)
{
	if (ItemData.ItemState != EItemState::EIS_InInventory) return;
	Coins += ItemData.ItemCost;
	UE_LOG(LogTemp, Warning, TEXT("Sold item price: %d"), ItemData.ItemCost);
	UE_LOG(LogTemp, Warning, TEXT("Sold Stack: %d" ), ItemData.StackCount);
	uint8 Index = 0;
	for (FItemData& Item : InventoryItems)
	{
		if (Item.ItemClass == ItemData.ItemClass)
		{
			InventoryItems.RemoveAt(Index);
			break;
		}
		++Index;
	}
	if (IsLocallyControlled())
	{
		UpdateCurrency(Coins);
		OnRep_InventoryItems();
	}
}




void AHorizonCharacter::AddDiamonds(int32 Value)
{
	Diamonds += Value;
	UE_LOG(LogTemp, Warning, TEXT("Added Diamonds: %d"), Diamonds);
}

void AHorizonCharacter::AddHealth(float Value)
{
	Health += Value;
	if (Health >= MaxHealth)
	{
		Health = MaxHealth;
	}
}

void AHorizonCharacter::ShowInventory(bool Visible)
{
	if(HorizonPlayerController)
	{
		HorizonPlayerController->ShowInventory(Visible);
	}
}

void AHorizonCharacter::StartSwimming()
{
	GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Swimming);
}

void AHorizonCharacter::StopSwimming()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = false;
}


void AHorizonCharacter::DrainFoodWater()
{
	Food = Food - 10.f;
	Water = Water - 10.f;
	if (Food <= 0 || Water <= 0)
	{
		//Destroy();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Thirst/Hunger has killed the player!"));
	}
}

void AHorizonCharacter::BeginPlay()
{
	Super::BeginPlay();

	HorizonPlayerController = Cast<AHorizonPlayerController>(GetController()); // We have a reference to the player controller , we can call functions like display inventory.

	TopMesh->SetMasterPoseComponent(GetMesh());
	BottomMesh->SetMasterPoseComponent(GetMesh());

	//Hunger/Thirst Initialization;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AHorizonCharacter::DrainFoodWater, FoodWaterDrainRate, true, 1.f);
}




void AHorizonCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AHorizonCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AHorizonCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHorizonCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AHorizonCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AHorizonCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AHorizonCharacter::ToggleInventory()
{
	if (HorizonPlayerController)
	{
		HorizonPlayerController->ToggleInventory();
	}

}


void AHorizonCharacter::UseItem(TSubclassOf<AItem> ItemSubclass)
{
	if (ItemSubclass)
	{
		if (HasAuthority())
		{
			if (AItem* Item = ItemSubclass.GetDefaultObject())
			{
				Item->Use(this);
			}
			uint8 Index = 0;
			for(FItemData& Item : InventoryItems)
			{
				if (Item.ItemClass == ItemSubclass)
				{
					--Item.StackCount;
					if (Item.StackCount <= 0)
					{
						InventoryItems.RemoveAt(Index);
					}
					break;
				}
				++Index;
			}
			if(IsLocallyControlled())
			{
				OnRep_InventoryItems();
			}
		}
		else
		{
			if (AItem* Item = ItemSubclass.GetDefaultObject())
			{
				Item->Use(this);
			}
			ServerUseItem(ItemSubclass);
		}
		
	}
}

bool AHorizonCharacter::ServerUseItem_Validate(TSubclassOf<AItem> ItemSubclass)
{
	return true;
}

void AHorizonCharacter::ServerUseItem_Implementation(TSubclassOf<AItem> ItemSubclass)
{
	for (FItemData& Item : InventoryItems)
	{
		if (Item.ItemClass == ItemSubclass)
		{
			if (Item.StackCount)
			{
				UseItem(ItemSubclass);
			}
			return;
		}
	}

}



void AHorizonCharacter::UpdateCurrency_Implementation(int32 InventoryCoins)
{
	
}

void AHorizonCharacter::AddInventoryItem(FItemData ItemData)
{
	if(HasAuthority())
	{
		ItemData.ItemState = EItemState::EIS_InInventory;
		bool bIsNewItem = true;
		for (FItemData& Item : InventoryItems) 
		{
			if (Item.ItemClass == ItemData.ItemClass) // Check if we already have item of this type.
			{
				if (Item.StackCount > 1)
				{
					Item.StackCount += ItemData.StackCount;
					Item.ItemCost += ItemData.ItemCost;
				}
				else
				{
					++Item.StackCount;
				}
				
				bIsNewItem = false; 
				break;
			}
		}
		if(bIsNewItem)
		{
			InventoryItems.Add(ItemData);
		}

		if (IsLocallyControlled())
		{
			OnRep_InventoryItems();
		}
	
	}
}

void AHorizonCharacter::OnRep_InventoryItems()
{
	if (InventoryItems.Num())
	{
		AddItemAndUpdateInventoryWidget(InventoryItems[InventoryItems.Num() - 1],InventoryItems);
	}
	else
	{
		AddItemAndUpdateInventoryWidget(FItemData(), InventoryItems);
	}
}
