// Fill out your copyright notice in the Description page of Project Settings.

#include "DelegateDispatcher.h"

// Sets default values
ADelegateDispatcher::ADelegateDispatcher(const FObjectInitializer& objectInitializer)
	:Super(objectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_collision = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollision"));

	_collision->SetSphereRadius(250.f);
	_collision->SetHiddenInGame(false);
	_collision->OnComponentBeginOverlap.AddDynamic(this, &ADelegateDispatcher::OnDelegateOverLap);
	_collision->OnComponentHit.AddDynamic(this, &ADelegateDispatcher::OnDelegateHit);

	RootComponent = _collision;
}

// Called when the game starts or when spawned
void ADelegateDispatcher::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(_loopTimerHandle, this, &ADelegateDispatcher::onTimerEnd, 5.f, false);
}

// Called every frame
void ADelegateDispatcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADelegateDispatcher::OnDelegateOverLap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Something hit me");

	AMainCharacter* overlappedCharacter = Cast<AMainCharacter>(OtherActor);

	if (overlappedCharacter->IsValidLowLevel())
	{
		overlappedCharacter->PlayerHealing.AddDynamic(this, &ADelegateDispatcher::WhenPlayerHeals);
	}
}

void ADelegateDispatcher::OnDelegateHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
}

void ADelegateDispatcher::onTimerEnd()
{
	GEngine->AddOnScreenDebugMessage(-1, 555.f, FColor::Red, "5 seconds goes through");

	GetWorld()->GetTimerManager().SetTimer(_loopTimerHandle, this, &ADelegateDispatcher::onTimerEnd, 5.f, false);
}

void ADelegateDispatcher::WhenPlayerHeals(FVector location)
{
	GEngine->AddOnScreenDebugMessage(-1, 19.f, FColor::Red, "Player is healing");
}

