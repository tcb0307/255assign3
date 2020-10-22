// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MainCharacter.h"
#include "AIEnemyController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerTakingDamage, float, healthReduction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerNearby, bool, executePlayer);

UCLASS()
class ASSIGNMENT1_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()

public: 
	AAIEnemyController();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* PawnGang) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual FRotator GetControlRotation() const override;

	UFUNCTION()
		void OnPawnDetected(const TArray<AActor*> &DetectedPawns);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightRadius = 500.0f; //AI Sighting Range of 500 units

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightAge = 5.0f;  // How long the AI sights last in intervals

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AILoseSightRadius = AISightRadius + 50.0f; //Loses the sight of the character in 50 units

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AIFieldOfView = 90.0f; //Field Of View

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool bIsPlayerDetected = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float DistanceToPlayer = 0.0f;

	UPROPERTY(BlueprintAssignable)
		FPlayerTakingDamage ReducingPlayerHealth;

	UPROPERTY(BlueprintAssignable)
		FPlayerNearby AttackNearbyPlayer;

private:
	UFUNCTION()
		void AttackPlayer(bool playerNearby);

};
