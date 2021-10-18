// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs.h"
#include "InteractInterface.h"
#include "Item.generated.h"



UCLASS()
class HORIZON_API AItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(EditDefaultsOnly, Category = "ItemMesh")
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FItemData ItemData;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FItemData GetItemData() const { return ItemData; }


	/* Implementation for the functions defined in the Interact Interface*/
	virtual void Interact(class AHorizonCharacter* Character) override;
	virtual void Use(AHorizonCharacter* Character) override;
};
