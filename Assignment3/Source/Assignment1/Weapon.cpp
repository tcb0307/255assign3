// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//initialise static mesh
	body = CreateDefaultSubobject<UStaticMeshComponent>("body");
	this->RootComponent = CollisionComponent;

	// Use these components to drive this projectile's movement
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(body);
	ProjectileMovementComponent->InitialSpeed = 9000.0f;
	ProjectileMovementComponent->MaxSpeed = 9000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->AddForce(struct FVector(200.0f, 200.0f, 200.0f));

	//collision component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("bullet collision");
	CollisionComponent->SetSphereRadius(1000);
	CollisionComponent->SetupAttachment(body);
	CollisionComponent->GetGenerateOverlapEvents();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnComponentBeginOverlap);

	// Die after 3 seconds.
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Function that initializes the projectile's velocity in the shoot direction.
void AWeapon::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OnComponentBeginOverlap, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Enemy"))
	{
		bool enemyCloseBy = true;
		EnemyDetection.Broadcast(enemyCloseBy);
	}
}