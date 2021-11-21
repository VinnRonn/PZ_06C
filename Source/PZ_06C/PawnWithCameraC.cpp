#include "PawnWithCameraC.h"

APawnWithCameraC::APawnWithCameraC()
{
	PrimaryActorTick.bCanEverTick = true;
	//init components
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");

	//StaticMeshComp->SetSimulatePhysics(true);
	//attach components
	StaticMeshComp->SetupAttachment(RootComponent);
	SpringArmComp->SetupAttachment(StaticMeshComp);
	CameraComp->SetupAttachment(SpringArmComp);
	//set location
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f),
		FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 400.0f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 2.0f;

	JumpLength = 100.0f;
	HightJump = 0.0f;
}


void APawnWithCameraC::BeginPlay()
{
	Super::BeginPlay();
	
}

void APawnWithCameraC::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APawnWithCameraC::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APawnWithCameraC::CameraPitch(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void APawnWithCameraC::CameraYaw(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void APawnWithCameraC::ZoomIn()
{
	bZoomingIn = true;
}

void APawnWithCameraC::ZoomOut()
{
	bZoomingIn = false;
}

void APawnWithCameraC::JumpIn()
{
	bJumping = true;
}

void APawnWithCameraC::JumpOut()
{
	bJumping = false;
}

void APawnWithCameraC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Zoom
	{
		if (bZoomingIn)
		{
			ZoomFactor += DeltaTime / 0.05f;
		}
		else
		{
			ZoomFactor -= DeltaTime / 0.025f;
		}
		ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);
		CameraComp->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
		SpringArmComp->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, ZoomFactor);
	}

	//Rotate out actor's yaw
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		SetActorRotation(NewRotation);
	}

	{
		FRotator NewRotation = SpringArmComp->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, -15.0f);
		SpringArmComp->SetWorldRotation(NewRotation);
	}

	{
		if (!MovementInput.IsZero())
		{
			MovementInput = MovementInput.GetSafeNormal() * 100.0f;
			FVector NewLocation = GetActorLocation();
			NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
			NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
			SetActorLocation(NewLocation);
			
		}
	}

	{
		if (bJumping)
		{
			FVector NewLocation = GetActorLocation();
			NewLocation.Z += DeltaTime / 0.005f;
			HightJump += DeltaTime / 0.005f;
			SetActorLocation(NewLocation);
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red,
				FString::SanitizeFloat(HightJump));
			SetActorLocation(NewLocation);
			if (HightJump > JumpLength)
			{
				bJumping = false;
			}
			
		}
		else if(HightJump > 0.0f)
		{
			FVector NewLocation = GetActorLocation();
			NewLocation.Z -= DeltaTime / 0.005f;
			HightJump -= DeltaTime / 0.005f;
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green,
				FString::SanitizeFloat(HightJump));
			SetActorLocation(NewLocation);
			HightJump = FMath::Clamp<float>(HightJump, 0.0f, JumpLength);
		}
	}
}


void APawnWithCameraC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &APawnWithCameraC::ZoomIn);
	InputComponent->BindAction("ZoomIn", IE_Released, this, &APawnWithCameraC::ZoomOut);
	
	InputComponent->BindAxis("MoveForward", this, &APawnWithCameraC::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APawnWithCameraC::MoveRight);
	InputComponent->BindAxis("CameraPitch", this, &APawnWithCameraC::CameraPitch);
	InputComponent->BindAxis("CameraYaw", this, &APawnWithCameraC::CameraYaw);

	InputComponent->BindAction("Jump", IE_Pressed, this, &APawnWithCameraC::JumpIn);
	
}

