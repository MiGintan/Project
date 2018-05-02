// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidget.h"
#include "target.generated.h"

/**
 * 
 */
UCLASS()
class SPACEVIKINGS_API Utarget : public UUserWidget
{
	GENERATED_BODY()

		virtual void NativeConstruct() override;
};
