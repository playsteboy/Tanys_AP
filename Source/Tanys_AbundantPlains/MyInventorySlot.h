// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "MyItemData.h"
#include "MyInventorySlot.generated.h"

USTRUCT(BlueprintType)
struct FMyInventorySlot
{
	GENERATED_BODY()

public:
	FMyInventorySlot()
		: ItemName(TEXT("None")), Quantity(1), ItemIcon(nullptr)
	{}
private:
	UMyItemData* ItemData;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	FName ItemName;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 Quantity;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	UTexture2D* ItemIcon;
public:
	virtual FName GetItemName() const { return ItemName; }
	virtual int32 GetQuantity() const { return Quantity; }
	virtual void SetQuantity(int32 value) { Quantity = value; }
	
};
