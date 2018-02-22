// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SavingAndLoading.generated.h"

UCLASS()
class SPACEVIKINGS_API ASavingAndLoading : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASavingAndLoading();

	void saveLoadData(FString& fileName, FArchive& ar, float& currentHealth,  float& maxHealth,  FVector& location);

	void saveSlotData(FString& slotName, FArchive& ar, bool& load);

	UFUNCTION(BlueprintCallable, Category = "Saving and Loading")
		bool saveToFile(const FString& fileName, const FString& slotName, const float& currentHealth,  const float& maxHealth, const FVector& location);

	UFUNCTION(BlueprintCallable, Category = "Saving and Loading")
		bool loadFromFile(FString& fileName, const FString& slotName, float& currentHealth, float& maxHealth, FVector& location);

	UFUNCTION(BlueprintCallable, Category = "Saving and Loading")
		bool doesFileExist(const FString& slotName);

	UFUNCTION(BlueprintCallable, Category = "Saving and Loading")
		bool loadSlotName(FString& slotName, bool& load);

	UFUNCTION(BlueprintCallable, Category = "Saving and Loading")
		bool saveSlotName(const FString& slotName, const bool& load);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	float playersMaxHealth;
	float playersCurrentHealth;
	FVector currentLocation;
	FString file;
};
