// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "PawnWithCameraC.generated.h"

UCLASS()
class PZ_06C_API APawnWithCameraC : public APawn
{
	GENERATED_BODY()

public:
	APawnWithCameraC();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComp;

	FVector2D MovementInput;
	FVector2D CameraInput;
	float ZoomFactor;
	float JumpLength;
	float HightJump;
	bool bZoomingIn;
	bool bJumping;


	void MoveForward(float AxisValue);

	void MoveRight(float AxisValue);

	void CameraPitch(float AxisValue);

	void CameraYaw(float AxisValue);

	void ZoomIn();
	void ZoomOut();

	void JumpIn();
	void JumpOut();
	
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
