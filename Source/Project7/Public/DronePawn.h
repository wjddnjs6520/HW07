// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DronePawn.generated.h"

//컴포넌트
class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;

// IA 입력 값 구조체
struct FInputActionValue;


UCLASS()
class PROJECT7_API ADronePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADronePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 캡슐 컴포넌트(루트). 블루프린트에서 읽기 가능(수정은 코드로). 카테고리 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UCapsuleComponent* CapsuleComp;

	// 캡슐 컴포넌트(루트). 블루프린트에서 읽기 가능(수정은 코드로). 카테고리 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* SkeletalComp;

	// 스프링 암 컴포넌트. 블루프린트에서 읽기 가능(수정은 코드로). 카테고리 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	// 카메라 컴포넌트. 블루프린트에서 읽기 가능(수정은 코드로). 카테고리 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	// 기본 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float CurrentSpeed; 
	// 지면 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float NormalSpeed; 
	// 줄이는 속도(배율)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float DownSpeed; 	
	// 날때 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float FlySpeed; 	

	// 중력 상수, 낙하 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float Gravity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float FallingSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	float MaxFallingSpeed;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	bool bIsFly; 	// 날고 있는지 여부

	// 드론 기울기 설정값
	UPROPERTY(EditAnywhere, Category = "Drone|Rotation")
	float MaxPitchAngle = 20.f;

	UPROPERTY(EditAnywhere, Category = "Drone|Rotation")
	float MaxRollAngle = 20.f;

	UPROPERTY(EditAnywhere, Category = "Drone|Rotation")
	float RotationInterpSpeed = 5.f;

	// 이동 입력값 저장용
	UPROPERTY(EditAnywhere, Category = "Drone|Rotation")
	FVector2D CurrentMoveValue = FVector2D::ZeroVector;


	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void UPDrone(const FInputActionValue& value);
	void DownDrone(const FInputActionValue& value);

	void GroundCheck();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
