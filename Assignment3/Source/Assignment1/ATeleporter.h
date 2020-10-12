// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "GameFramework/Actor.h"
#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "ATeleporter.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT1_API AATeleporter : public ATriggerBox
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;


public:
	AATeleporter();

	UFUNCTION()
		void EnterTeleporter(class AActor* overlappedActor, class AActor* otherActor);
	
	UFUNCTION()
		void ExitTeleporter(class AActor* overlappedActor, class AActor* otherActor);

	UPROPERTY(EditAnywhere, Category = "Teleporter")
		AATeleporter* otherTele;

	UPROPERTY()
		bool teleporting;
};
