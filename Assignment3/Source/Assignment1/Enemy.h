// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Weapon.h"
#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttackNearbyPlayer2, bool, attackPlayer2);

UCLASS()
class ASSIGNMENT1_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	bool pDead; //determine if the character is that or not

	UPROPERTY(EditAnywhere, BluePrintReadwrite)
		UBoxComponent* CollisionComponent;

	UPROPERTY(BlueprintAssignable)
		FAttackNearbyPlayer2 PlayerUnderAttack2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Dead();

	void DestroyEnemy();

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OnComponentBeginOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void AvoidProjectile(bool projectilePresence);
};
