// Fill out your copyright notice in the Description page of Project Settings.

#include "AIEnemyController.h"
#include "AIEnemy.h"
#include "Waypoint.h"
#include "MainCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAIEnemyController::AAIEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true; 
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; //Sight Settings to detect anything

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation()); //Dominant Settings is set to Sight (Could make it sound etc if wanted more)
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAIEnemyController::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

}

void AAIEnemyController::BeginPlay()
{
	Super::BeginPlay();
}

void AAIEnemyController::OnPossess(APawn* PawnGang)
{
	Super::OnPossess(PawnGang);
}

void AAIEnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AAIEnemy* MyCharacter = Cast<AAIEnemy>(GetPawn());

	if (DistanceToPlayer > AISightRadius)
	{
		bIsPlayerDetected = false;
	}

	//Move to Waypoint
	if (MyCharacter->NextWaypoint != nullptr && bIsPlayerDetected == false)
	{
		MoveToActor(MyCharacter->NextWaypoint, 4.0f);
	}
	
	else if (bIsPlayerDetected == true)
	{
		AMainCharacter* Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		MoveToActor(Player, 5.0f);

		//Attack player when player is nearby
		MyCharacter->PlayerUnderAttack.AddDynamic(this, &AAIEnemyController::AttackPlayer);
	}

}

FRotator AAIEnemyController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AAIEnemyController::OnPawnDetected(const TArray<AActor*> &DetectedPawns)
{
	for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);
	}

	bIsPlayerDetected = true;
}

void AAIEnemyController::AttackPlayer(bool playerNearby)
{
	float playerDamage = 20.f;
	ReducingPlayerHealth.Broadcast(playerDamage);
}