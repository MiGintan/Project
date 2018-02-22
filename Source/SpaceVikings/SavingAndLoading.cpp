// Fill out your copyright notice in the Description page of Project Settings.

#include "SavingAndLoading.h"
#include "PlatformFilemanager.h"
#include "BufferArchive.h"
#include "FileHelper.h"
#include "PlatformProcess.h"
#include "MemoryReader.h"
#include "Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include <MyCharacter.h>


// Sets default values
ASavingAndLoading::ASavingAndLoading()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UBoxComponent* boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	boxComponent->SetBoxExtent(FVector(100, 100, 32));
	boxComponent->AttachTo(RootComponent);

	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASavingAndLoading::OnOverlapBegin);
}

void ASavingAndLoading::saveLoadData(FString& fileName, FArchive& ar, float& currentHealth, float& maxHealth, FVector& location)
{
	ar << currentHealth;
	ar << location;
	ar << fileName;
	ar << maxHealth;
}

void ASavingAndLoading::saveSlotData(FString& slotName, FArchive& ar, bool& load)
{
	ar << slotName;
	ar << load;
}

bool ASavingAndLoading::saveToFile(const FString& fileName, const FString& slotName, const float& currentHealth, const float& maxHealth, const FVector& location)
{
	FBufferArchive toBinary;
	FString projectDir = FPlatformProcess::UserDir();
	FString fullPathName = projectDir + "/Project/Space Vikings/Saves/" + slotName + ".sav";
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	playersCurrentHealth = currentHealth;
	playersMaxHealth = maxHealth;
	currentLocation = location;
	file = fileName;

	saveLoadData(file, toBinary, playersCurrentHealth, playersMaxHealth, currentLocation);

	if (toBinary.Num() <= 0)
		return false;

	if (!PlatformFile.DirectoryExists(*projectDir))
		PlatformFile.CreateDirectory(*projectDir);

	if (FFileHelper::SaveArrayToFile(toBinary, *fullPathName))
	{
		toBinary.FlushCache();
		toBinary.Empty();
	}
	else
	{
		// Free Binary Array 	
		toBinary.FlushCache();
		toBinary.Empty();
	}

	return false;
}

bool ASavingAndLoading::loadFromFile(FString& fileName, const FString& slotName, float& currentHealth, float& maxHealth, FVector& location)
{
	FBufferArchive theBinaryArray;
	FString projectDir = FPlatformProcess::UserDir();
	FString fullPathName = projectDir + "/Project/Space Vikings/Saves/" + slotName + ".sav";

	if (!FFileHelper::LoadFileToArray(theBinaryArray, *fullPathName))
	{
		return false;
	}

	if (theBinaryArray.Num() <= 0)
		return false;

	FMemoryReader fromBinary = FMemoryReader(theBinaryArray, true); //true, free data after done
	fromBinary.Seek(0);
	saveLoadData(fileName, fromBinary, currentHealth, maxHealth, currentLocation);

	fromBinary.FlushCache();

	// Empty & Close Buffer 
	theBinaryArray.Empty();
	fromBinary.Close();

	return true;
}

bool ASavingAndLoading::doesFileExist(const FString& slotName)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString projectDir = FPlatformProcess::UserDir();
	FString fullPathName = projectDir + "/Project/Space Vikings/Saves/" + slotName + ".sav";

	if (PlatformFile.DirectoryExists(*projectDir))
	{
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*fullPathName))
		{
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool ASavingAndLoading::loadSlotName(FString& slotName, bool& load)
{
	FBufferArchive theBinaryArray;
	FString projectDir = FPlatformProcess::UserDir();
	FString fullPathName = projectDir + "/Project/Space Vikings/Saves/Slots/slotName.sav";

	if (!FFileHelper::LoadFileToArray(theBinaryArray, *fullPathName))
	{
		return false;
	}

	if (theBinaryArray.Num() <= 0)
		return false;

	FMemoryReader fromBinary = FMemoryReader(theBinaryArray, true); //true, free data after done
	fromBinary.Seek(0);
	saveSlotData(slotName, fromBinary, load);

	fromBinary.FlushCache();

	// Empty & Close Buffer 
	theBinaryArray.Empty();
	fromBinary.Close();

	return true;
}

bool ASavingAndLoading::saveSlotName(const FString& slotName, const bool& load)
{
	FBufferArchive toBinary;
	FString projectDir = FPlatformProcess::UserDir();
	FString fullPathName = projectDir + "/Project/Space Vikings/Saves/Slots/slotName.sav";
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	FString slot;

	slot = slotName;
	bool loadGame = load;

	saveSlotData(slot, toBinary, loadGame);

	if (toBinary.Num() <= 0)
		return false;

	if (!PlatformFile.DirectoryExists(*projectDir))
		PlatformFile.CreateDirectory(*projectDir);

	if (FFileHelper::SaveArrayToFile(toBinary, *fullPathName))
	{
		toBinary.FlushCache();
		toBinary.Empty();
	}
	else
	{
		// Free Binary Array 	
		toBinary.FlushCache();
		toBinary.Empty();
	}

	return false;
}

void ASavingAndLoading::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, .5f, FColor::Yellow, TEXT("Saving"));

		saveSlotName("Test", false);
		saveToFile("TestFile", "Test", 5, 10, FVector(0,0,0));

		GEngine->AddOnScreenDebugMessage(-1, .5f, FColor::Green, TEXT("File Saved"));
	}
}