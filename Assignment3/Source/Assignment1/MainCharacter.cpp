// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "LineTrace.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "AIEnemyController.h" 
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Get the capsule component, because this is a pointer so we need InitCapsuleSize
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	//Radar around the player, used for trigerring other actors for when to attack player
	//ActorsNearby = CreateDefaultSubobject<USphereComponent>(TEXT("Actors Nearby"));
	//ActorsNearby->SetupAttachment(RootComponent);
	//ActorsNearby->InitSphereRadius(126.0f);

	//These 3 codes execute to make sure the character itself cannot rotate, only allow the camera to rotate
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; //this will allow character movement to rotate in the direction where it is moving 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); //rotation rate for X, Y, Z
	GetCharacterMovement()->JumpZVelocity = 600.0f; //when the character jumps the velocity will be 600
	GetCharacterMovement()->AirControl = 0.2f; //how much can we control the character when in the air

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); //create spring arm component in Camera
	CameraBoom->SetupAttachment(RootComponent); //attach to the root which is the capsule component

	CameraBoom->TargetArmLength = 300.0f; //How far away the camera go from the main body
	CameraBoom->bUsePawnControlRotation = true; //this is going to rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	//this is going to attach the camera at the end of the boom and let the boom adjust the match controller rotation of the camera
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false; //the camera does not rotate relative to the arm

	LineTraceComponent = CreateDefaultSubobject<ULineTrace>("CreateDefaultSubobject");

	pDead = false;
	Health = 100.0f;

	WalkingSpeed = 600.0f;
	RunningSpeed = 1500.0f;

}


// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetCurrentState(EMainCharacterState::ENormal);

	//We have to bind capsule component from the our own capsule component with the on begin overlap, in order to trigger when the character collided with a pawn
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnBeginOverLap); 

	//ActorsNearby->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnOtherActorBeginOverLap);
	
	if (Character_Health_Widget_Class != nullptr) //fix if the health bar is not equal to null pointer
	{
		Character_Health_Widget = CreateWidget(GetWorld(), Character_Health_Widget_Class);
		Character_Health_Widget->AddToViewport();
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("health: %s"), Health);
	//Health -= DeltaTime * Health_Treshold;

	if (Health <= 70 && Health > 0)
	{
		SetCurrentState(EMainCharacterState::EPowerful);
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}

	else if (Health <= 0)
	{
		if (!pDead)
		{
			SetCurrentState(EMainCharacterState::EDead);
			pDead = true;

			GetMesh()->SetSimulatePhysics(true);

			FTimerHandle UnusedHandle;
			//so GetWorldTimerManager is going to set the timer by using UnusedHandle as a paarameter inside this class and AMainCharacter is the function I want to execute after 3s and false to not repeated it
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMainCharacter::RestartGame, 3.0f, false);
		}
	}

	else 
	{
		SetCurrentState(EMainCharacterState::ENormal);
		GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput); //Bind the "turn" in Input and bind into the Pawn, allow the player to rotate, Yaw is to change Z rotation
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput); //Pitch is to rotate Y rotation

	//bind Press and Release as well
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump); //these are built-in functions inside of the character
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AMainCharacter::Shoot);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

}

void AMainCharacter::MoveForward(float val)
{
	if (!pDead) //happens when the character is not dead
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0); //0 for X-asix, Rotation.Yaw for Y-axis and 0 for Z-axis

		//this is going to calculate the forward direction of this vector from the yaw rotation vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); 
		AddMovementInput(Direction, val);
	}
}

void AMainCharacter::MoveRight(float val)
{
	if (!pDead)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0); //0 for X-asix, Rotation.Yaw for Y-axis and 0 for Z-axis
		//this is going to calculate the right direction of this vector from the yaw rotation vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, val);
	}
}

void AMainCharacter::Shoot()
{
	const UWorld* world = this->GetWorld();
	if (world)
	{
		FActorSpawnParameters sparams;
		sparams.Owner = this;
		sparams.Instigator = this;

		//get socket location and rotation
		TArray<FName> sockets = GetMesh()->GetAllSocketNames();

		if (sockets[0] == TEXT("BulletSocket"))
		{
			FVector SocketLocation = GetMesh()->GetSocketLocation(sockets[0]); //getting SocketLocation
			FRotator SocketRotation = GetMesh()->GetSocketRotation(sockets[0]); //getting SocketRotation
			FVector Start = FVector(GetMesh()->GetBoneLocation(FName("head"))); //Bullet goes from charater's head
			FVector End = SocketLocation + GetMesh()->GetRightVector() * 2000.0f; //The lazer go away 2000.0f
			FHitResult HitResult = LineTraceComponent->LineTraceSingle(Start, End, true); //Showing Lanetrace
			AWeapon* bullet = GetWorld()->SpawnActor<AWeapon>(BP_Bullet,
				SocketLocation, SocketRotation, sparams); //Bullet pop up when I shoot
			FCollisionQueryParams CollisionParameters;
			if (HitResult.GetActor()) //Debug when bullet is hitting something
			{
				if (HitResult.GetActor()->GetName().Equals("Enemy"))
				{
					UE_LOG(LogTemp, Warning, TEXT("HITTING ENEMY"));
				}
				UE_LOG(LogTemp, Warning, TEXT("HIT: %s"), *HitResult.GetActor()->GetName());
			}

		}
	}
}

void AMainCharacter::RestartGame() //Restart the game when the character is dead
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

EMainCharacterState AMainCharacter::GetCurrentState() const
{
	return CurrentState;
}

void AMainCharacter::SetCurrentState(EMainCharacterState NewState)
{
	CurrentState = NewState;
}

void AMainCharacter::OnBeginOverLap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Health"))
	{
		Health += 60.0f; //player's health + 10

		if (Health > 100.0f)
			Health = 100.0f;

		OtherActor->Destroy(); //destroy healthitem

		FVector	Xd = { 10.f, 20.f, 30.f };

		PlayerHealing.Broadcast(Xd);
	}

	if (OtherActor->ActorHasTag("Enemy"))
	{
		AAIEnemyController* Enemy = Cast<AAIEnemyController>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		//Attack player when player is nearby
		Enemy->AttackNearbyPlayer.AddDynamic(this, &AMainCharacter::EnemyCloseBy);
	}
}

void AMainCharacter::EnemyCloseBy(bool attackPlayer)
{
	Health -= 10.0f;
	//UE_LOG(LogTemp, Warning, TEXT("Taking Damage!"));
}