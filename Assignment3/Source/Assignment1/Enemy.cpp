// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>("collision component");
	CollisionComponent->SetBoxExtent(FVector(200, 200, 200));
	CollisionComponent->GetGenerateOverlapEvents();
	CollisionComponent->SetupAttachment(GetMesh());
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnComponentBeginOverlap);

	pDead = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::DestroyEnemy()
{
	this->Destroy();
}

void AEnemy::Dead()
{
	pDead = true;

	GetMesh()->SetSimulatePhysics(true);

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AEnemy::DestroyEnemy, 3.0f, false);
}

void AEnemy::OnComponentBeginOverlap(UPrimitiveComponent* OnComponentBeginOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AWeapon::StaticClass()))
	{
		//cast the other actor to bullet
		AWeapon* bullet = Cast<AWeapon>(OtherActor);

		bullet->EnemyDetection.AddDynamic(this, &AEnemy::AvoidProjectile);

		//destroy the enemy
		this->Dead();
		bullet->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
	}

	if (OtherActor->ActorHasTag("Player"))
	{
		bool attackPlayer = true;
		PlayerUnderAttack2.Broadcast(attackPlayer);
	}
}

void AEnemy::AvoidProjectile(bool projectilePresence)
{
	if (projectilePresence == true)
	{
		// Code where the enemy reacts to the incoming projectile
	}
}