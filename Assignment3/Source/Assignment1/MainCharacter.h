// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "MainCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerHeal, FVector, location);

UENUM(BlueprintType)
enum class EMainCharacterState : uint8
{
	ENormal,
	EPowerful,
	EDead
};

UCLASS()
class ASSIGNMENT1_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;

	void MoveForward(float val);

	void MoveRight(float val);

	void Shoot();

	bool pDead; //determine if the character is that or not

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Health;

	UPROPERTY(EditAnywhere)
		float Health_Treshold;

	UPROPERTY(EditAnywhere)
		float RunningSpeed;

	UPROPERTY(EditAnywhere)
		float WalkingSpeed;
		
	UFUNCTION()
		void OnBeginOverLap(class UPrimitiveComponent* HitComp, class AActor* HealthItem, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditAnywhere, Category = "UI HUD")
		TSubclassOf<UUserWidget> Character_Health_Widget_Class;
	UUserWidget* Character_Health_Widget;

	void RestartGame();

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
		FOnPlayerHeal PlayerHealing;
	
	UFUNCTION(BlueprintPure, Category = "Power")
		EMainCharacterState GetCurrentState() const;

	// Set a new playing state
	void SetCurrentState(EMainCharacterState NewState);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//archetype bullet blueprint class
	UPROPERTY(Category = "Weapon", EditAnywhere, BlueprintReadOnly)
		TSubclassOf<AWeapon> BP_Bullet;

private:
	// Keeps track of the current playing state
	EMainCharacterState CurrentState;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class ULineTrace* LineTraceComponent;
};