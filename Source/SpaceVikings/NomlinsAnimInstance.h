// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NomlinsAnimInstance.generated.h"

UCLASS()
class SPACEVIKINGS_API UNomlinsAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool isInAir;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool attacking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bite;
};
